目前只写了rgb灯的程序用来查看电路搭建是否正确，分别有三个态：
    *   **待机/呼吸态 (IDLE)**: `CRGB::PowderBlue` (淡蓝色)
    *   **触发态 (TRIGGERED)**: `CRGB::Orange` (橙色)
    *   **成功态 (SUCCESS)**: `CRGB::Green` (绿色)
    *   **警告/失败态 (WARNING)**: `CRGB::Red` (红色)
    用的灯是WS2812B
