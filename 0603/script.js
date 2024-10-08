document.addEventListener('DOMContentLoaded', fetchDevicesStatus);

// Hàm khôi phục trạng thái thiết bị
function fetchDevicesStatus() {
    fetch('api.php', {
        method: 'GET',
        credentials: 'include' // Gửi thông tin session
    })
    .then(response => response.json())
    .then(devices => {
        document.getElementById('light-status').innerText = `Trạng thái: ${devices.light ? 'Bật' : 'Tắt'}`;
        document.getElementById('ac-status').innerText = `Trạng thái: ${devices.ac ? 'Bật' : 'Tắt'}`;
        document.getElementById('curtain-status').innerText = `Trạng thái: ${devices.curtain ? 'Mở' : 'Đóng'}`;
        document.getElementById('security-status').innerText = `Trạng thái: ${devices.security ? 'Đã kích hoạt' : 'Vô hiệu hóa'}`;
        document.getElementById('camera-status').innerText = `Trạng thái: ${devices.camera ? 'Bật' : 'Tắt'}`;
    })
    .catch(error => console.error('Lỗi khi lấy trạng thái thiết bị:', error));
}

// Hàm cập nhật trạng thái thiết bị
function updateDeviceStatus(action, statusElementId, statusTextOn, statusTextOff) {
    fetch('api.php', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ action })
    })
    .then(response => response.json())
    .then(data => {
        alert(data.message);
        const statusElement = document.getElementById(statusElementId);
        const isOn = action.includes('on') || action.includes('open') || action.includes('activate');
        statusElement.innerText = `Trạng thái: ${isOn ? statusTextOn : statusTextOff}`;
    })
    .catch(error => console.error('Lỗi:', error));
}

// Hàm xử lý lệnh giọng nói
function handleVoiceCommands(commands) {
    fetch('api.php', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ action: 'control_devices', commands })
    })
    .then(response => response.json())
    .then(data => {
        data.messages.forEach(message => {
            alert(message); // Hiển thị thông báo cho từng lệnh
            speakMessage(message); // Phát âm thông báo nếu cần
        });
        fetchDevicesStatus(); // Cập nhật lại trạng thái thiết bị
    })
    .catch(error => console.error('Lỗi:', error));
}

// Thiết lập nhận diện giọng nói
if ('webkitSpeechRecognition' in window) {
    const recognition = new webkitSpeechRecognition();
    recognition.continuous = false; // Không nhận diện liên tục
    recognition.interimResults = false; // Không hiển thị kết quả tạm thời
    recognition.lang = 'vi-VN'; // Đặt ngôn ngữ là tiếng Việt

    // Khi nhận diện thành công
    recognition.onresult = function(event) {
        const transcript = event.results[0][0].transcript.toLowerCase();
        console.log('Kết quả nhận diện:', transcript);
        handleVoiceCommands([transcript]);
    };

    // Khi có lỗi
    recognition.onerror = function(event) {
        console.error('Lỗi nhận diện giọng nói:', event.error);
    };

    // Bắt đầu nhận diện giọng nói khi nhấn nút
    document.getElementById('voice-command').addEventListener('click', () => {
        recognition.start();
        console.log('Đang lắng nghe...');
    });
} else {
    console.log("Web Speech API không được hỗ trợ trên trình duyệt này.");
}

// Các sự kiện cho các nút điều khiển
document.getElementById('light-on').addEventListener('click', () => {
    updateDeviceStatus('light_on', 'light-status', 'Bật', 'Tắt');
});
document.getElementById('light-off').addEventListener('click', () => {
    updateDeviceStatus('light_off', 'light-status', 'Bật', 'Tắt');
});
document.getElementById('ac-on').addEventListener('click', () => {
    updateDeviceStatus('ac_on', 'ac-status', 'Bật', 'Tắt');
});
document.getElementById('ac-off').addEventListener('click', () => {
    updateDeviceStatus('ac_off', 'ac-status', 'Bật', 'Tắt');
});
document.getElementById('curtain-open').addEventListener('click', () => {
    updateDeviceStatus('curtain_open', 'curtain-status', 'Mở', 'Đóng');
});
document.getElementById('curtain-close').addEventListener('click', () => {
    updateDeviceStatus('curtain_close', 'curtain-status', 'Mở', 'Đóng');
});
document.getElementById('security-activate').addEventListener('click', () => {
    updateDeviceStatus('security_activate', 'security-status', 'Đã kích hoạt', 'Vô hiệu hóa');
});
document.getElementById('security-deactivate').addEventListener('click', () => {
    updateDeviceStatus('security_deactivate', 'security-status', 'Đã kích hoạt', 'Vô hiệu hóa');
});
document.getElementById('camera-on').addEventListener('click', () => {
    updateDeviceStatus('camera_on', 'camera-status', 'Bật', 'Tắt');
});
document.getElementById('camera-off').addEventListener('click', () => {
    updateDeviceStatus('camera_off', 'camera-status', 'Bật', 'Tắt');
});

// Hàm để phát âm thông báo (nếu cần)
function speakMessage(message) {
    const utterance = new SpeechSynthesisUtterance(message);
    window.speechSynthesis.speak(utterance);
}