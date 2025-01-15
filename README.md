# "LoTrix" - Lofi Matrix Effect

Decided to make this fun little thing just to learn a bit of C++ and Raylib.

Still a noob at it but I will get there (eventually).

Font made by me.

## Arguments & defaults
  - `./LoTrix [width] [height] [max_amount] [rainbow] [message]`
    - `[width]/[height]`: Width and height it'll be drawn at. Default is 128 by 128.
    - `[max_amount]`: Maximum amount of characters that will be created. Default is the highest value between `[width]` and `[height]` divided by 4 (128 / 4 = 32).
    - `[rainbow]`: Fun colors. Default is set to false.
    - `[message]`: Display a character or a whole message on the screen using "". Default is empty.
- Keybinds:
  - `KEY_SPACE/KEY_P` - Pause/unpause.
  - `KEY_F` - Toggle fullscreen.

## Images

![](preview1.png)

`./LoTrix 512 288 128 false`

(35MB~).

---
![](preview2.png)

`./LoTrix 512 288 128 true`

(35MB~).

---
![](previewmsg.png)

`./LoTrix 512 288 256 true`

(38MB~)
