import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

# -------------------
# 設定
# -------------------
SERIAL_PORT = 'COM8'   # 実際のM5StickCのポートに変更
BAUD_RATE = 115200
MAX_DATA_POINTS = 50   # グラフに表示する最新データの数
UPDATE_INTERVAL = 200  # ミリ秒単位の更新間隔
# -------------------

# データ保持用リスト
data_points = []
time_points = []
start_time = time.time()

# Matplotlib 初期設定
plt.style.use('ggplot')
fig, ax = plt.subplots()
line, = ax.plot([], [], label="Brightness", color="orange", linewidth=2)
ax.set_title("M5StickC Brightness Real-Time Plot")
ax.set_xlabel("Time (s)")
ax.set_ylabel("Brightness")
ax.set_ylim(0, 1000)  # 初期Y軸
ax.legend()
ax.grid(True)

# -------------------
# シリアル接続
# -------------------
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    print(f"Serial port {SERIAL_PORT} opened successfully.")
except Exception as e:
    print(f"Error opening serial port: {e}")
    exit()

# -------------------
# アニメーション更新関数
# -------------------
def update(frame):
    try:
        # シリアルから読み込み
        while ser.in_waiting:
            line_bytes = ser.readline()
            line_str = line_bytes.decode('utf-8').strip()
            if line_str.isdigit():
                val = int(line_str)
                t = time.time() - start_time
                data_points.append(val)
                time_points.append(t)
                
                # データが多すぎたら古いものを削除
                if len(data_points) > MAX_DATA_POINTS:
                    data_points.pop(0)
                    time_points.pop(0)
    except Exception as e:
        print(f"Serial read error: {e}")

    # グラフ更新
    line.set_data(time_points, data_points)
    if time_points:
        ax.set_xlim(time_points[0], time_points[-1] + 0.1)
        y_min = min(data_points)
        y_max = max(data_points)
        ax.set_ylim(max(0, y_min - 50), y_max + 50)
    return line,

# -------------------
# アニメーション開始
# -------------------
ani = animation.FuncAnimation(
    fig,
    update,
    interval=UPDATE_INTERVAL,
    cache_frame_data=False  # 警告回避
)

# -------------------
# グラフ表示
# -------------------
plt.show()

# 終了処理
ser.close()
print("Serial connection closed.")
