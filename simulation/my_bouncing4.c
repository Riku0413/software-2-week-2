#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "my_physics4.h"

int main(int argc, char **argv) {
    const Condition cond =
        {
            .width  = 75,
            .height = 40,
            .k = 1.0, // クーロン定数
            .dt = 1.0, // 何秒刻みで何秒刻みで時間が進むか
            .cor = 0.8 // 反発係数
        };

    FILE *fp = fopen(argv[2], "r");
    size_t objnum = atoi(argv[1]);
    Object objects[objnum];

    char s[objnum][100];
    while (1) {
      fgets(s[0], 100, fp);
      if (s[0][0] != '#') {
        break;
      }
    }

    for (int i = 0; i < objnum; i++) { // ある行をピックアップ
      int s_p = 0;
      char t[20];
      double data_list[6];
      for (int k = 0; k < 6; k++) { // ある物理量をピックアップ
        int t_p = 0;
        while (s[i][s_p] != ' ' && s[i][s_p] != '\n') {
          t[t_p] = s[i][s_p];
          s_p++;
          t_p++;
        }
        t[t_p] = '\0';
        s_p++;
        data_list[k] = atof(t);
      }

      objects[i].m = data_list[0];
      objects[i].q = data_list[1];
      objects[i].x = data_list[2];
      objects[i].vx = data_list[3];
      objects[i].y = data_list[4];
      objects[i].vy = data_list[5];

      if (fgets(s[i+1], 100, fp) == NULL) {
        for (int i2 = i + 1; i2 < objnum; i2++) {
          objects[i2].m = 0;
          objects[i2].q = 0;
          objects[i2].x = cond.width + i2;
          objects[i2].vx = 0;
          objects[i2].y = cond.height + i2;
          objects[i2].vy = 0;
        }
        break;
      }
    }

    // シミュレーション. ループは整数で回しつつ、実数時間も更新する
    const double stop_time = 400;
    double t = 0;
    for (size_t i = 0 ; t <= stop_time ; i++){
        t = i * cond.dt;
        my_update_velocities(objects, objnum, cond);
        my_update_positions(objects, objnum, cond);
        my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる

        my_fusion(objects, objnum, cond);
        
        // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
        my_plot_objects(objects, objnum, t, cond);
        
        usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
        printf("\e[%dA", cond.height+3+(int)objnum);// 壁とパラメータ表示分で3行
    }
    return EXIT_SUCCESS;
}