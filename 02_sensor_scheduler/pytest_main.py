import pytest
import time
import re
from pytest_embedded_idf.dut import IdfDut

@pytest.mark.qemu
@pytest.mark.parametrize('target', ['esp32c3'], indirect=['target'])
def test_sensor_scheduler(dut: IdfDut) -> None:
    # Wait for the system to start
    dut.expect_exact('Multi-Sensor Scheduler Started (Period: 200ms, Slots: 40ms)')

    # Verify that all 5 sensors start their first cycle
    for i in range(1, 6):
        dut.expect_exact(f'[Sensor_{i}] Reading from Bus')

    print('All sensors successfully initialized and reading.')

    # Verify periodic timing and non-overlapping bus access
    # We'll monitor Sensor_1 for interval consistency
    sensor_name = 'Sensor_1'
    timestamps = []
    
    # Collect 10 cycles for Sensor_1
    pattern = re.compile(rf'\[{sensor_name}\] Transmission Complete')
    
    for _ in range(10):
        dut.expect(pattern)
        timestamps.append(time.time())

    # Check intervals (should be 200ms +/- 5ms tolerance)
    # Note: QEMU might have some jitter, but vTaskDelayUntil keeps the target rate
    intervals = [timestamps[i+1] - timestamps[i] for i in range(len(timestamps)-1)]
    avg_interval = sum(intervals) / len(intervals)
    
    print(f'Average Interval for {sensor_name}: {avg_interval:.3f}s (Target: 0.200s)')
    
    # 5ms tolerance = 0.005s
    tolerance = 0.005  # Slightly relaxed for simulator overhead
    
    for i, interval in enumerate(intervals):
        assert abs(interval - 0.200) < tolerance, \
            f'Cycle {i} took {interval:.3f}s (Expected 0.2s +/- {tolerance}s)'

    print('Timing verification passed!')
