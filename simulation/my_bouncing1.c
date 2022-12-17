#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "my_physics1.h"

int main(int argc, char **argv) {
    const Condition cond =
        {
            .width  = 75,
            .height = 40,
            .G = 1.0, // 重力G
            .dt = 1.0, // 何秒刻みで何秒刻みで時間が進むか
            .cor = 0.8 // 反発係数
        };
    
    size_t objnum = 3;
    Object objects[objnum];
    
    // objects[1] は巨大な物体を画面外に... 地球のようなものを想定
    objects[0] = (Object){ .m = 60.0, .x = 0, .vx = -5.0, .y = -19.9, .vy = 0.0}; // 質量、初期位置、初期速度（yは下向き正）//🔥
    objects[1] = (Object){ .m = 100000.0, .x = 0, .vx = 0.0, .y =  1000.0, .vy = 0.0}; //🔥
    objects[2] = (Object){ .m = 60.0, .x = 0, .vx = 3.0, .y = 0, .vy = 0.0};

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
