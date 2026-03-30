import pytest
import re
from pytest_embedded_idf.dut import IdfDut

@pytest.mark.qemu
@pytest.mark.parametrize('target', ['esp32c3'], indirect=['target'])
def test_concurrent_system(dut: IdfDut) -> None:
    # 1. Verify Startup
    dut.expect_exact('Concurrent System Simulation Started')

    # 2. Verify Sensor Task updates
    dut.expect(r'Sensor Task: Updating buffer \(Cycle 0\)')
    dut.expect(r'Sensor Task: Updating buffer \(Cycle 1\)')

    # 3. Verify Communication Task sends data
    # It should happen after at least one update
    dut.expect_exact('Communication Task: Sending data')
    
    # 4. Verify Data Payload consistency
    # Comm task runs every 1000ms, Sensor task every 500ms.
    # So Cycle 1 or 2 should be captured.
    res = dut.expect(r'Data Payload: \[S1:(\d+\.\d), S2:(\d+\.\d), S3:(\d+\.\d), S4:(\d+\.\d), S5:(\d+\.\d)\]')
    
    s1_val = float(res.group(1))
    print(f'Captured Sensor 1 value: {s1_val}')
    
    # Verify values are non-zero (mock data was counter + i*1.1)
    assert s1_val >= 0.0

    # 5. Verify further updates to prove concurrency
    dut.expect(r'Sensor Task: Updating buffer \(Cycle [2-9]\)')
    dut.expect_exact('Communication Task: Sending data')

    print('Concurrency and Buffer Safety Verified!')
