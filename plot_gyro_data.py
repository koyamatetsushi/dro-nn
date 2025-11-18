import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

# --- 設定 ---
# ご自身のM5StickC Plus2が接続されているシリアルポートに合わせて変更してください
# Windows: 'COM3', 'COM4' など
# macOS/Linux: '/dev/tty.usbserial-XXXX', '/dev/ttyUSB0' など
SERIAL_PORT = 'COM8' 

# シリアル通信のボーレート (Arduinoスケッチと合わせる)
BAUD_RATE = 115200

# グラフに保持するデータポイントの最大数
MAX_DATA_POINTS = 100 
# -----------------

# データ保持用のリスト (X, Y, Zの3軸分)
time_points = []
gx_points = []
gy_points = []
gz_points = []
start_time = time.time()

# Matplotlibの初期設定
plt.style.use('ggplot')
fig, ax = plt.subplots(figsize=(10, 6))
fig.suptitle("M5StickC Plus2 Gyro Sensor Real-Time Plot (deg/s)", fontsize=16)

# 3軸のラインプロットを初期化
line_x, = ax.plot(time_points, gx_points, label="X-Axis (Roll)", color='r')
line_y, = ax.plot(time_points, gy_points, label="Y-Axis (Pitch)", color='g')
line_z, = ax.plot(time_points, gz_points, label="Z-Axis (Yaw)", color='b')

ax.set_xlabel("Time (s)")
ax.set_ylabel("Angular Velocity (deg/s)")
ax.legend(loc='upper right')
# ジャイロデータの一般的な範囲 (-500 から 500)
ax.set_ylim(-500, 500) 

# シリアルポートの初期化
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.05)
    print(f"Serial port {SERIAL_PORT} opened successfully.")
except serial.SerialException as e:
    print(f"Error opening serial port {SERIAL_PORT}: {e}")
    print("Please check the port name and ensure M5StickC Plus2 is connected.")
    exit()

def read_and_plot(i):
    """
    アニメーションフレームごとに呼び出される関数。
    シリアルポートからデータを読み込み、リストを更新して、グラフを再描画します。
    """
    try:
        if ser.in_waiting > 0:
            # 1行読み込み、デコードし、前後の空白や改行を削除
            line_bytes = ser.readline()
            line_str = line_bytes.decode('utf-8').strip()
            
            # カンマ区切りで値を分割
            parts = line_str.split(',')
            
            # データが3つあり、全て数値であることを確認
            if len(parts) == 3 and all(p.replace('.', '', 1).replace('-', '', 1).isdigit() for p in parts):
                
                # 浮動小数点数に変換
                gx = float(parts[0])
                gy = float(parts[1])
                gz = float(parts[2])
                current_time = time.time() - start_time
                
                # データをリストに追加
                time_points.append(current_time)
                gx_points.append(gx)
                gy_points.append(gy)
                gz_points.append(gz)
                
                # リストが最大データポイント数を超えたら古いものを削除
                if len(time_points) > MAX_DATA_POINTS:
                    time_points.pop(0)
                    gx_points.pop(0)
                    gy_points.pop(0)
                    gz_points.pop(0)

                # グラフを更新
                line_x.set_data(time_points, gx_points)
                line_y.set_data(time_points, gy_points)
                line_z.set_data(time_points, gz_points)
                
                # X軸の範囲を最新のデータに合わせて自動更新
                if time_points:
                    ax.set_xlim(time_points[0], time_points[-1] + 0.1)
                
                # Y軸の範囲をデータに合わせて動的に調整
                all_data = gx_points + gy_points + gz_points
                if all_data:
                    y_min = min(all_data)
                    y_max = max(all_data)
                    # ゼロを中心にマージンを設ける
                    margin = max(abs(y_min), abs(y_max)) * 0.1 
                    ax.set_ylim(y_min - margin, y_max + margin)
                
    except Exception as e:
        # 読み取り中にエラーが発生した場合の処理
        print(f"An error occurred during reading or plotting: {e}")
        
# アニメーションの開始 (intervalはArduino側のdelayと同期させる)
ani = animation.FuncAnimation(fig, read_and_plot, interval=50, cache_frame_data=False)

# グラフを表示 (このウィンドウを閉じるとプログラムが終了します)
plt.show()

# 終了処理
ser.close()
print("Application closed. Serial connection terminated.")
