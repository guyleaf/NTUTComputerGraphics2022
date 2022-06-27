# Computer Graphics - final project (part1)

[TOC]

## TODO

* 實作基本功能
  * [x] File 讀四個 obj 檔功能
  * [x] Render Mode 切換功能
  * [x] Color Mode 切換功能
  * [x] Bounding Box 開關
* [x] 支援 command line 讀取 obj 檔案
* [x] 自動依照模型大小縮放至適合的顯示大小
* [x] 動畫功能
* [x] FOV 縮放功能 (*extra)

## 如何執行

1. 打開 OpenGLFinal.sln 專案

2. 直接執行 或 切至 Release 建置專案

   #### Release 建置專案

   1. 建置專案後，去 x64/Release 底下複製 OpenGLFinal.exe 至[這裡](OpenGLFinal) (必須複製到 resources 資料夾的上一層，不然會造成 obj 找不到檔案的錯誤)
   2. 直接點開 exe 或打開 cmd 輸入 `OpenGLFinal.exe {你想要載入的 obj 檔案}`
      例如: `OpenGLFinal.exe resources/teapot.obj`

## 操作功能說明

1. 動畫 開始/暫停 功能，按下 keyboard `p` 或 `P`
2. 滑鼠滾輪縮放功能(*)，控制 FOV 來進行縮放

### 選單

1. File 選單，可選擇載入內建四種模型檔案 (因內建的 Open File... 的 dialog 是限定 Windows 平台使用，所以就沒多實作可選擇其他檔案)
   ![image-20220627210345935](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627210345935.png)

2. Render Mode 選單，可選擇 Point、Line、Face 三種模型呈現方式
   ![image-20220627210528777](C:\Users\ychhu\AppData\Roaming\Typora\typora-user-images\image-20220627210528777.png)

3. Color Mode 選單，可選擇 Fixed Color (White)、Random Color (每點一次 randomize 一次)
   ![image-20220627210630775](C:\Users\ychhu\AppData\Roaming\Typora\typora-user-images\image-20220627210630775.png)

4. Bounding Box 開關，是否顯示 bounding box
   ![image-20220627210702425](C:\Users\ychhu\AppData\Roaming\Typora\typora-user-images\image-20220627210702425.png)

## 截圖、影片

* Line
  ![image-20220627213319436](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213319436.png)
* Point
  ![image-20220627213335496](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213335496.png)
* Face
  ![image-20220627213757258](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213757258.png)

* Random Color

  ![image-20220627213409517](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213409517.png)![image-20220627213427705](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213459799.png)

* Bounding Box

  ![image-20220627213529215](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213529215.png)![image-20220627213540963](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213540963.png)![image-20220627213600959](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627213600959.png)

* Command Line 載入指定 obj 檔案

  ![image-20220627214016575](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627214016575.png)

* 動畫開始/暫停、自動縮放模型至合適大小，[影片](https://youtu.be/s3fVi9ZTtNE)

* FOV 滑鼠滾輪縮放，[影片](https://youtu.be/X5UfUwwUEvU)

* 完整 Demo，[影片](https://youtu.be/fWNY77Xoyzc)

## 遇到的問題

* 如何在不知道 object 模型的大小情況下進行 scaling 縮放?
  * ❌搭配 glOrth 去 clip object 後，再設定 glViewport，完整放入到畫面中，缺點是需要提前知道 object 的大小
  * ⭕使用 glScalef 與搭配找到的 bounding box，計算 scale factor。
    原本沒想那麼多，直接依照三邊長與要映射的大小(targetSize)相除後，再個別丟入對應的 glScalef 參數，但這會造成 object 縮放變形，因為沒有等比例縮放。例如下圖，teapot 變形
    
    ![image-20220627215503696](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627215503696.png)
    
    所以正確解法是: 求出 width, height, depth -> 找出三邊中的最大值 -> 與 targetSize 相除，統一求出共同的 scale factor。
    
    ![image-20220627215552232](E:\Git\NTUTComputerGraphics2022\OpenGLFinal\Docs\image-20220627215552232.png)

