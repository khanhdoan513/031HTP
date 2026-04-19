import time
from pymodbus.client import ModbusSerialClient
from pymodbus.payload import BinaryPayloadDecoder
from pymodbus.constants import Endian

# Cấu hình cổng kết nối
PORT = 'COM10'
BAUDRATE = 9600
SLAVE_ID = 45

# Khởi tạo client
client = ModbusSerialClient(
    port=PORT,
    baudrate=BAUDRATE,
    timeout=0.1, # Timeout nên nhỏ hơn hoặc bằng interval
    stopbits=1,
    bytesize=8,
    parity='N'
)

def read_imu_data():
    addr = 56;
    if client.connect():
        print("✅ Đã kết nối Modbus RTU. Bắt đầu đọc dữ liệu (Ctrl+C để dừng)... \n")
        try:
            while True:
                # Đọc 2 thanh ghi từ địa chỉ 56
                response = client.read_holding_registers(address=addr, count=2, slave=SLAVE_ID)
                # addr += 1
                if not response.isError():
                    # Giải mã dữ liệu float 32-bit
                    decoder = BinaryPayloadDecoder.fromRegisters(
                        response.registers,
                        byteorder=Endian.BIG,
                        wordorder=Endian.LITTLE
                    )
                    imu_angle = decoder.decode_32bit_float()
                    
                    # \r và end="" giúp cập nhật giá trị trên cùng một dòng cho gọn
                    print(f"\r👉 IMU Angle: {imu_angle:.3f}          ", end="", flush=True)
                else:
                    print("\n❌ Lỗi truyền thông: Không thể đọc thanh ghi")

                # Nghỉ 100ms (0.1 giây)
                # time.sleep(0.1)
                # break
        except KeyboardInterrupt:
            print("\n\n🛑 Đã dừng chương trình bởi người dùng.")
        finally:
            client.close()
            print("🔌 Đã đóng cổng kết nối.")
    else:
        print("❌ Không thể mở cổng Serial")

if __name__ == "__main__":
    read_imu_data()