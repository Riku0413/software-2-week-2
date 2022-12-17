// シミュレーション条件を格納する構造体
// 反発係数CORを追加
typedef struct condition {
    int width; // 見えている範囲の幅
    int height; // 見えている範囲の高さ
    double G; // 重力定数
    double dt; // シミュレーションの時間幅
    double cor; // 壁の反発係数
} Condition;

// 個々の物体を表す構造体
typedef struct object {
    double m;
    double y; // 最新時刻の物理量？
    double prev_y; // 壁からの反発に使用 // 一個前の時刻の物理量？
    double vy;
    double prev_vy;
} Object;

// 授業で用意した関数のプロトタイプ宣言

void plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
void update_velocities(Object objs[], const size_t numobj, const Condition cond);
void update_positions(Object objs[], const size_t numobj, const Condition cond);
void bounce(Object objs[], const size_t numobj, const Condition cond);

// To do: 以下に上記と同じ引数で実行される my_* を作成
// 実装できたらmain関数で上記と入れ替えていく
// my_plot_objects(), my_update_velocities(), my_update_positions(), my_bounce の4つ

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i < numobj; i++) {
        double a = 0;
        for (int j = 0; j < numobj; j++) {
            if (j == i) {
                continue; // 自分からの引力はない
            }
            a += objs[j].m * (objs[j].y - objs[i].y) / pow(fabs(objs[j].y - objs[i].y), 3.0);
        }
        a *= cond.G;
        objs[i].prev_vy = objs[i].vy;
        objs[i].vy += a * cond.dt;
    }
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i < numobj; i++) {
        objs[i].prev_y = objs[i].y;
        objs[i].y += objs[i].prev_vy * cond.dt;
    }
}

void my_bounce(Object objs[], const size_t numobj, const Condition cond) {
    double y_min = - cond.height/2;
    double y_max = cond.height/2;
    for (int i = 0; i < numobj; i++) {
        if (objs[i].prev_y > y_min && objs[i].prev_y < y_max && objs[i].y > y_max) {
            double dt1 = (y_max - objs[i].prev_y)/objs[i].prev_vy;
            double dt2 = cond.dt - dt1;
            objs[i].vy *= -cond.cor;
            objs[i].y = y_max + objs[i].prev_vy * (-cond.cor) * dt2;
        } else if (objs[i].prev_y > y_min && objs[i].prev_y < y_max && objs[i].y < y_min) {
            double dt1 = (objs[i].prev_y - y_min)/fabs(objs[i].prev_vy);
            double dt2 = cond.dt - dt1;
            objs[i].vy *= -cond.cor;
            objs[i].y = y_min + objs[i].prev_vy * (-cond.cor) * dt2;
        }
    }
}

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond) {

    printf("+");
    for (int j = 0; j < cond.width; j++) {
        printf("-");
    }
    printf("+\r\n"); // \r は要る？

    for (int i = 0; i < cond.height; i++) {
        printf("|");
        for (int j = 0; j < cond.width; j++) {
            printf(" ");
            for (int k = 0; k < numobj; k++) {
                if (cond.height/2 + round(objs[k].y) == i && j == (cond.width + 1)/2) {
                    printf("\bo"); // 空白を一個消した上で物体をプロット
                    break;
                } else if (cond.height/2 + round(objs[k].y) == 40 && i == 39 && j == (cond.width + 1)/2) {
                    printf("\bo"); // y = 40 のときに物体が消失しないように調整
                    break;
                }
            }
        }
        printf("|\r\n");
    }

    printf("+");
    for (int j = 0; j < cond.width; j++) {
        printf("-");
    }
    printf("+\r\n");

    printf("t = %lf, ", t);
    for (int k = 0; k < numobj; k++) {
        printf("objs[%d].y = %lf", k, objs[k].y);
        if (k != numobj - 1) {
            printf(", ");
        }
    }
    printf("\r\n");

}