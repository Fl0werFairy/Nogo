# Nogo
基于终端界面的“不围棋”，是pku 2022计算概率A课程的大作业。（个人认为远远超出了课程范围，毫无意义。）
## 玩法说明
采用`stdin`输入，输出到`stdout`。具有存档、读档的简单功能。具体说明可参看游戏内部菜单。
## （算法）实现说明
按照“当前局面下可以落子的格点数目”，较大时采取MCT(蒙特卡洛+UCT估值)算法，较小(<10)时采用悲观的minimax算法。
- 由于算法不够完善，当程序通过minimax算法计算出当前局面下必输时，可能会落子在违反规则处！
### 补充说明
代码未提供在botzone平台上的json交互功能。
