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
typedef struct object { //🔥
    double m;
    double y; // 最新時刻の物理量？
    double prev_y; // 壁からの反発に使用 // 一個前の時刻の物理量？
    double vy;
    double prev_vy;
    double x;
    double prev_x;
    double vx;
    double prev_vx;
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
        double a_x = 0; //🔥
        double a_y = 0; //🔥
        for (int j = 0; j < numobj; j++) {
            if (j == i) {
                continue; // 自分からの引力はない
            }
            a_x += objs[j].m * (objs[j].x - objs[i].x) / pow(pow(objs[j].x - objs[i].x, 2) + pow(objs[j].y - objs[i].y, 2), 1.5); //🔥
            a_y += objs[j].m * (objs[j].y - objs[i].y) / pow(pow(objs[j].x - objs[i].x, 2) + pow(objs[j].y - objs[i].y, 2), 1.5); //🔥
        }
        a_x *= cond.G; //🔥
        objs[i].prev_vx = objs[i].vx; //🔥
        objs[i].vx += a_x * cond.dt; //🔥
        a_y *= cond.G; //🔥
        objs[i].prev_vy = objs[i].vy; //🔥
        objs[i].vy += a_y * cond.dt; //🔥
    }
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i < numobj; i++) {
        objs[i].prev_x = objs[i].x; //🔥
        objs[i].prev_y = objs[i].y; //🔥
        objs[i].x += objs[i].prev_vx * cond.dt; //🔥
        objs[i].y += objs[i].prev_vy * cond.dt; //🔥
    }
}

void my_bounce(Object objs[], const size_t numobj, const Condition cond) {
    double x_min = - cond.width/2; //🔥 // ← ここらへん怪しい -17.5
    double x_max = cond.width/2; //🔥
    double y_min = - cond.height/2; //🔥
    double y_max = cond.height/2; //🔥
    for (int i = 0; i < numobj; i++) { // x方向、y方向、は完全に独立に扱えそう！
        if (objs[i].prev_x > x_min && objs[i].prev_x < x_max && objs[i].x > x_max) {
            double dt1 = (x_max - objs[i].prev_x)/objs[i].prev_vx;
            double dt2 = cond.dt - dt1;
            objs[i].vx *= -cond.cor;
            objs[i].x = x_max + objs[i].prev_vx * (-cond.cor) * dt2;
        } else if (objs[i].prev_x > x_min && objs[i].prev_x < x_max && objs[i].x < x_min) {
            double dt1 = (objs[i].prev_x - x_min)/fabs(objs[i].prev_vx);
            double dt2 = cond.dt - dt1;
            objs[i].vx *= -cond.cor;
            objs[i].x = x_min + objs[i].prev_vx * (-cond.cor) * dt2;
        } //🔥
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
        } //🔥
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
                if (cond.height/2 + objs[k].y >= i && cond.height/2 + objs[k].y < i + 1 && cond.width/2 + objs[k].x >= j && cond.width/2 + objs[k].x < j + 1) { //🔥
                    printf("\bo"); // 空白を一個消した上で物体をプロット
                    break;
                } //🔥
            }
        }
        printf("|\r\n");
    }

    printf("+");
    for (int j = 0; j < cond.width; j++) {
        printf("-");
    }
    printf("+\r\n");

    printf("t = %lf\r\n", t);
    for (int k = 0; k < numobj; k++) {
        printf("objs[%d].x = %lf, objs[%d].y = %lf", k, objs[k].x, k, objs[k].y); //🔥
        if (k != numobj - 1) {
            printf("\n");
        }
    }
    printf("\r\n");

}