from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class FW01ScaffoldTests(unittest.TestCase):
    def test_root_cmake_uses_esp_idf_project(self):
        text = (ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
        self.assertIn("$ENV{IDF_PATH}/tools/cmake/project.cmake", text)
        self.assertIn("project(homegrid_fw01)", text)

    def test_component_registers_c_source(self):
        text = (ROOT / "main" / "CMakeLists.txt").read_text(encoding="utf-8")
        self.assertIn('SRCS "app_main.c"', text)

    def test_source_reports_chip_and_runtime_metadata(self):
        text = (ROOT / "main" / "app_main.c").read_text(encoding="utf-8")
        for required in (
            "esp_chip_info(&chip_info)",
            "esp_get_idf_version()",
            "chip_info.revision",
            "chip_info.cores",
            "CHIP_FEATURE_WIFI_BGN",
            "CHIP_FEATURE_BLE",
            "esp_get_free_heap_size()",
            "esp_get_minimum_free_heap_size()",
        ):
            with self.subTest(required=required):
                self.assertIn(required, text)

    def test_heartbeat_uses_freertos_delay(self):
        text = (ROOT / "main" / "app_main.c").read_text(encoding="utf-8")
        self.assertIn("vTaskDelay(pdMS_TO_TICKS(10000))", text)
        self.assertIn('"Heartbeat;', text)

    def test_no_hardware_or_network_initialization_in_fw01(self):
        text = (ROOT / "main" / "app_main.c").read_text(encoding="utf-8").lower()
        forbidden = (
            "gpio_set_level(",
            "gpio_config(",
            "esp_wifi_init(",
            "esp_ble_gap_start_advertising(",
            "esp_mqtt_client_init(",
            "relay_on",
            "switch_on",
        )
        for token in forbidden:
            with self.subTest(token=token):
                self.assertNotIn(token, text)

    def test_codex_guardrails_and_board_worksheet_exist(self):
        self.assertTrue((ROOT / "AGENTS.md").is_file())
        self.assertTrue((ROOT / "docs" / "board-identification.md").is_file())
        self.assertTrue((ROOT / "CODEX_FIRST_TASK.md").is_file())


if __name__ == "__main__":
    unittest.main()
