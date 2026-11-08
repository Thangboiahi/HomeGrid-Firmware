"""FW-02F source-level safety boundary regression checks."""

from pathlib import Path
import re
import unittest


ROOT = Path(__file__).resolve().parents[1]
SOURCE_ROOTS = (
    ROOT / "components" / "homegrid_domain",
    ROOT / "components" / "homegrid_mock",
    ROOT / "components" / "homegrid_application",
    ROOT / "components" / "homegrid_diagnostics",
)
FORBIDDEN_INCLUDES = re.compile(
    r'^\s*#\s*include\s*[<"]'
    r'(?:driver/gpio|esp_wifi|esp_now|mqtt|nvs|nvs_flash|'
    r'esp_ota_ops|esp_https_ota)(?:/|[.>"])',
    re.MULTILINE,
)
FORBIDDEN_CALLS = re.compile(
    r'\b(?:gpio_config|gpio_set_level|gpio_reset_pin|'
    r'esp_wifi_init|esp_now_init|esp_mqtt_client_init|'
    r'nvs_open|esp_ota_begin)\s*\('
)


class FW02SafetyAuditTests(unittest.TestCase):
    def test_expected_source_components_exist(self):
        for source_root in SOURCE_ROOTS:
            self.assertTrue(source_root.is_dir(), str(source_root))
            self.assertTrue(
                any(source_root.rglob("*.c")),
                f"No C implementation found under {source_root}",
            )

    def test_fw02_sources_have_no_forbidden_platform_includes_or_calls(self):
        for source_root in SOURCE_ROOTS:
            for path in (*source_root.rglob("*.c"), *source_root.rglob("*.h")):
                text = path.read_text(encoding="utf-8")
                self.assertIsNone(
                    FORBIDDEN_INCLUDES.search(text),
                    f"Forbidden platform include in {path}",
                )
                self.assertIsNone(
                    FORBIDDEN_CALLS.search(text),
                    f"Forbidden hardware/network/storage/OTA call in {path}",
                )

    def test_output_permission_enum_is_deny_only(self):
        header = (
            ROOT / "components" / "homegrid_domain" / "include"
            / "homegrid" / "switch_state.h"
        ).read_text(encoding="utf-8")
        match = re.search(
            r"typedef\s+enum\s*\{([^}]+)\}\s*"
            r"hg_switch_output_permission_t\s*;",
            header,
            re.DOTALL,
        )
        self.assertIsNotNone(match, "Output permission enum not found")
        values = re.findall(r"\bHG_SWITCH_[A-Z0-9_]+\b", match.group(1))
        self.assertEqual(values, ["HG_SWITCH_OUTPUT_DENIED"])

    def test_mock_is_the_only_actuator_component(self):
        mock_header = (
            ROOT / "components" / "homegrid_mock" / "include"
            / "homegrid" / "mock_actuator.h"
        )
        self.assertTrue(mock_header.is_file())
        self.assertFalse((ROOT / "components" / "homegrid_gpio").exists())
        self.assertFalse((ROOT / "components" / "homegrid_relay").exists())


if __name__ == "__main__":
    unittest.main()
