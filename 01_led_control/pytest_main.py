import pytest
import time
import re
from pytest_embedded_idf.dut import IdfDut

@pytest.mark.qemu
@pytest.mark.parametrize('target', ['esp32c3'], indirect=['target'])
def test_led_simulation(dut: IdfDut) -> None:
    # 1. Verify Initialization logs
    dut.expect_exact('LED Red initialized (Active: LOW, Freq: 10.0 Hz)')
    dut.expect_exact('LED Green initialized (Active: HIGH, Freq: 5.0 Hz)')
    dut.expect_exact('LED Blue initialized (Active: HIGH, Freq: 2.0 Hz)')

    # 2. Verify Manual Test Sequence
    dut.expect_exact('Starting Manual Test Sequence...')
    dut.expect_exact('[Red] Logical: ON | Physical: 0')   # Active LOW
    dut.expect_exact('[Red] Logical: OFF | Physical: 1')  # Active LOW
    print('Manual ON/OFF verification passed!')

    # 3. Verify Blink Simulation Start
    dut.expect_exact('Starting LED Blink Simulation...')

    # 4. Verify Blink Timing (Red LED - 10Hz -> 50ms toggle)
    # Target: 10Hz = 10 cycles/sec = 20 toggles/sec = 0.05s per toggle
    verify_blink_timing(dut, 'Red', expected_interval=0.05, tolerance=0.02, count=10)
    
    # 5. Verify Blink Timing (Green LED - 5Hz -> 100ms toggle)
    verify_blink_timing(dut, 'Green', expected_interval=0.10, tolerance=0.03, count=5)

    print('Blink timing verification passed!')

def verify_blink_timing(dut: IdfDut, led_name: str, expected_interval: float, tolerance: float, count: int) -> None:
    """
    Helper to verify that an LED toggles at the expected interval.
    """
    print(f'Verifying timing for {led_name} (Expected: {expected_interval}s)...')
    timestamps = []
    pattern = re.compile(rf'\[{led_name}\] Logical: (ON|OFF)')
    
    for _ in range(count):
        dut.expect(pattern)
        timestamps.append(time.time())
    
    intervals = [timestamps[i+1] - timestamps[i] for i in range(len(timestamps)-1)]
    avg_interval = sum(intervals) / len(intervals)
    
    print(f'{led_name} Average Interval: {avg_interval:.3f}s (Target: {expected_interval}s)')
    
    for i, interval in enumerate(intervals):
        assert abs(interval - expected_interval) < tolerance, \
            f'{led_name} toggle {i} took {interval:.3f}s (Expected {expected_interval}s +/- {tolerance}s)'
