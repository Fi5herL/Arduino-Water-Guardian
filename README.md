# Arduino Water Guardian

以下是程式碼的說明以及 Relay 的接線方式：

**程式碼說明：**

1.  **感測器與腳位定義：**
    *   `analogInPin` (A0): 連接水感測器的類比輸入腳位。
    *   `relayPin` (7): 連接繼電器的數位輸出腳位。
    *   `ledPin` (13): 連接一個 LED 的腳位，用來視覺化繼電器狀態。
    *   `testPin` (2): 這個腳位在 `setup()` 中被設為 `HIGH`，但在 `loop()` 中沒有被使用，可能是預留給測試或其他用途。

2.  **參數設定：**
    *   `windowSize` (50): 偵測時讀取的樣本數。程式會連續讀取 `windowSize` 次感測器的值。
    *   `threshold` (80): 水量偵測的閾值。如果讀取的類比值低於此閾值，則被視為水量存在。
    *   `triggerCount` (40): 觸發條件。只有當讀取到 `triggerCount` 次（含）以上的值低於 `threshold` 時，才認為偵測到水。
    *   `debounceTime` (1000): 延遲時間（毫秒）。為了避免感測器數值快速波動導致繼電器頻繁切換，程式會要求在狀態改變後，需要持續維持一段時間（在此為 1000ms，也就是 1 秒）才會正式改變繼電器狀態。

3.  **變數：**
    *   `zeroCount`: 記錄讀取到值低於 `threshold` 的次數。
    *   `relayState`: 記錄繼電器的目標狀態 (true = ON, false = OFF)。
    *   `lastDetected`: 記錄上一次偵測到的水是否存在狀態。
    *   `stateStartTime`: 記錄當前狀態開始的時間，用於 debouncing。

4.  **`setup()` 函數：**
    *   初始化序列埠通訊 (`Serial.begin(9600)`)，方便在序列埠監控器上查看輸出。
    *   設定 `relayPin`、`ledPin` 和 `testPin` 為輸出模式。
    *   將繼電器和 LED 初始化為關閉狀態 (`LOW`)。
    *   將 `testPin` 設定為 `HIGH`。

5.  **`loop()` 函數：**
    *   **讀取感測器值：** 迴圈 `windowSize` 次，每次讀取 `analogInPin` 的值，並檢查是否低於 `threshold`。低於 `threshold` 的話，`zeroCount` 就會增加。
    *   **判斷是否偵測到水：** `waterDetected` 變數會根據 `zeroCount` 是否達到 `triggerCount` 來設定。
    *   **Debouncing 邏輯：**
        *   如果偵測到的水狀態 (`waterDetected`) 與上一次的狀態 (`lastDetected`) 不同，則記錄當前時間 (`stateStartTime`) 並更新 `lastDetected`。
        *   只有當當前時間 (`millis()`) 減去 `stateStartTime` 大於或等於 `debounceTime` 時，才正式更新 `relayState`。
    *   **控制繼電器和 LED：** 根據 `relayState` 的值，HIGH 或 LOW 輸出到 `relayPin` 和 `ledPin`。
    *   **序列埠輸出：** 在序列埠監控器上顯示 `zeroCount`、`waterDetected` 和 `relayState` 的狀態。

**Wokwi 接線圖說明：**

![](![](./Circuit%202026-05-13.jpeg)

*   Wokwi 圖中的水位感測器為替代元件，用來表示實際 12V 水位感測器的接線邏輯。
*   實機時，水位感測器請使用外部 12V 電源供電。
*   外部 12V 電源的 GND 必須與 Arduino GND 共地。
*   接到 Arduino A0 的類比訊號電壓不可超過 5V。

**Relay 負載側Relay接點說明：**

*   **COM (Common):** 這是 Relay 的公共接點。
*   **NO (Normally Open):** 這個接點在 Relay 未通電時是斷開的。
*   **NC (Normally Closed):** 這個接點在 Relay 未通電時是閉合的（在此程式碼中沒有使用）。

**接線方式：**

1.  **電源線 (L - 火線):** 將您要控制的負載的電源線 L (火線) **剪斷**。
2.  **連接 COM:** 將火線剪斷後，其中一端（來自電源供應端）連接到 Relay 的 **COM** 接點。
3.  **連接 NO:** 將火線剪斷後，另一端（前往負載端）連接到 Relay 的 **NO** 接點。
4.  **中性線 (N):** 負載的中性線 (N) 則直接連接到負載，不需要經過 Relay。

**簡單圖示：**

```
      電源供應端 ------> COM (Relay) ------> L (連接入電端)
                         NO (Relay) ------> L (連接出電端)
```

**當 Arduino 程式讓 Relay 繼電器線圈通電時：**

*   Relay 的 COM 接點會被連接到 NO 接點。
*   這樣，電源的火線 L 就會通過 Relay，到達負載，負載就會通電工作。

**當 Arduino 程式讓 Relay 繼電器線圈斷電時：**

*   Relay 的 COM 接點會與 NO 接點斷開。
*   負載的電源線路就斷開，負載停止工作。

**注意事項：**

*   **Relay 模組：** 通常您會使用一個帶有驅動電路的 Relay 模組，這樣 Arduino 的 5V 或 3.3V 訊號就可以直接驅動 Relay。請確保您的 Relay 模組的驅動電壓與 Arduino 的輸出電壓匹配。
*   **電源：** 確保您要控制的負載的電源（AC 或 DC）和功率符合 Relay 的額定規格，以免損壞 Relay 或引起危險。
*   **安全：** 在進行任何電氣連接之前，請務必斷開所有電源，確保安全。如果您不熟悉電氣操作，請尋求專業人士的協助。
*   **程式碼中的 `testPin`：** 如果 `testPin` 沒有被使用，可以考慮移除相關的宣告和設定，讓程式碼更簡潔。
*   **`debounceTime`：** 程式碼中設定的 `debounceTime` 是 1000ms (1秒)。如果您的水感測器波動比較劇烈，可能需要增加這個數值。

希望這些說明對您有幫助！
