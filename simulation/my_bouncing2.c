#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "my_physics2.h"

int main(int argc, char **argv) {
    const Condition cond =
        {
            .width  = 75,
            .height = 40,
            .G = 1.0, // 重力G
            .dt = 1.0, // 何秒刻みで何秒刻みで時間が進むか
            .cor = 0.8 // 反発係数
        };
        
    FILE *fp = fopen(argv[2], "r");
    size_t objnum = atoi(argv[1]);
    Object objects[objnum];
    for (int i = 0; i < objnum; i++) {
        if (fscanf(fp, "%lf %lf %lf %lf %lf", &objects[i].m, &objects[i].x, &objects[i].vx, &objects[i].y, &objects[i].vy) == EOF) {
            objects[i].m = 0.0; objects[i].x = cond.width; objects[i].vx = 0.0; objects[i].y = cond.height; objects[i].vy = 0.0;
        } // 盤面の"常に外側の場所に"質量0の物体を置く
    }

    // シミュレーション. ループは整数で回しつつ、実数時間も更新する
    const double stop_time = 400;
    double t = 0;
    for (size_t i = 0 ; t <= stop_time ; i++){
        t = i * cond.dt;
        my_update_velocities(objects, objnum, cond);
        my_update_positions(objects, objnum, cond);
        my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる
        
        // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
        my_plot_objects(objects, objnum, t, cond);
        
        usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
        printf("\e[%dA", cond.height+3+(int)objnum);// 壁とパラメータ表示分で3行 //🔥
    }
    return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト
