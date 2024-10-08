<?php
session_start();
header("Content-Type: application/json");

// Khởi tạo trạng thái thiết bị nếu chưa có trong session
if (!isset($_SESSION['devices'])) {
    $_SESSION['devices'] = [
        "light" => false,
        "ac" => false,
        "curtain" => false,
        "security" => false,
        "camera" => false
    ];
}

// Lấy trạng thái từ session
$devices = &$_SESSION['devices'];

// Lấy phương thức HTTP (GET hoặc POST)
$method = $_SERVER['REQUEST_METHOD'];

switch ($method) {
    case 'GET':
        // Trả về trạng thái thiết bị hiện tại
        echo json_encode($devices);
        break;

    case 'POST':
        $data = json_decode(file_get_contents("php://input"), true);
        
        if (isset($data['action'])) {
            switch ($data['action']) {
                // Các hành động bật/tắt thiết bị
                case 'light_on':
                    $devices['light'] = true;
                    echo json_encode(["message" => "Đã bật đèn", "devices" => $devices]);
                    break;
                case 'light_off':
                    $devices['light'] = false;
                    echo json_encode(["message" => "Đã tắt đèn", "devices" => $devices]);
                    break;
                case 'ac_on':
                    $devices['ac'] = true;
                    echo json_encode(["message" => "Đã bật điều hòa", "devices" => $devices]);
                    break;
                case 'ac_off':
                    $devices['ac'] = false;
                    echo json_encode(["message" => "Đã tắt điều hòa", "devices" => $devices]);
                    break;
                case 'curtain_open':
                    $devices['curtain'] = true;
                    echo json_encode(["message" => "Rèm đã được mở", "devices" => $devices]);
                    break;
                case 'curtain_close':
                    $devices['curtain'] = false;
                    echo json_encode(["message" => "Rèm đã được đóng", "devices" => $devices]);
                    break;
                case 'security_activate':
                    $devices['security'] = true;
                    echo json_encode(["message" => "Hệ thống bảo mật đã được kích hoạt", "devices" => $devices]);
                    break;
                case 'security_deactivate':
                    $devices['security'] = false;
                    echo json_encode(["message" => "Hệ thống bảo mật đã được vô hiệu hóa", "devices" => $devices]);
                    break;
                case 'camera_on':
                    $devices['camera'] = true;
                    echo json_encode(["message" => "Camera đã được bật", "devices" => $devices]);
                    break;
                case 'camera_off':
                    $devices['camera'] = false;
                    echo json_encode(["message" => "Camera đã được tắt", "devices" => $devices]);
                    break;
                    
                // Xử lý nhiều lệnh cùng lúc
                case 'control_devices':
                    $commands = $data['commands']; // Nhận danh sách lệnh
                    $responses = [];

                    foreach ($commands as $command) {
                        $command = strtolower($command);
                        if (strpos($command, 'bật đèn') !== false) {
                            $devices['light'] = true;
                            $responses[] = "Đèn đã bật";
                        } elseif (strpos($command, 'tắt đèn') !== false) {
                            $devices['light'] = false;
                            $responses[] = "Đèn đã tắt";
                        } elseif (strpos($command, 'bật điều hòa') !== false) {
                            $devices['ac'] = true;
                            $responses[] = "Điều hòa đã bật";
                        } elseif (strpos($command, 'tắt điều hòa') !== false) {
                            $devices['ac'] = false;
                            $responses[] = "Điều hòa đã tắt";
                        } elseif (strpos($command, 'mở rèm') !== false) {
                            $devices['curtain'] = true;
                            $responses[] = "Rèm đã mở";
                        } elseif (strpos($command, 'đóng rèm') !== false) {
                            $devices['curtain'] = false;
                            $responses[] = "Rèm đã đóng";
                        } elseif (strpos($command, 'kích hoạt bảo mật') !== false) {
                            $devices['security'] = true;
                            $responses[] = "Hệ thống bảo mật đã được kích hoạt";
                        } elseif (strpos($command, 'vô hiệu hóa bảo mật') !== false) {
                            $devices['security'] = false;
                            $responses[] = "Hệ thống bảo mật đã được vô hiệu hóa";
                        } elseif (strpos($command, 'bật camera') !== false) {
                            $devices['camera'] = true;
                            $responses[] = "Camera đã được bật";
                        } elseif (strpos($command, 'tắt camera') !== false) {
                            $devices['camera'] = false;
                            $responses[] = "Camera đã được tắt";
                        }
                    }

                    echo json_encode(["messages" => $responses, "devices" => $devices]);
                    break;

                default:
                    echo json_encode(["message" => "Hành động không hợp lệ"]);
                    break;
            }
        } else {
            echo json_encode(["message" => "Không tìm thấy hành động"]);
        }
        break;

    default:
        echo json_encode(["message" => "Phương thức không được hỗ trợ"]);
        break;
}

error_log(print_r($_SESSION, true));
?>