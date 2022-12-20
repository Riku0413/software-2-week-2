// シミュレーション条件を格納する構造体
// 反発係数CORを追加
typedef struct condition {
    int width;
    int height;
    double k; // クーロン定数
    double dt;
    double cor;
} Condition;

// 個々の物体を表す構造体
typedef struct object {
    double m;
    double q; // 電荷量
    double y;
    double prev_y;
    double vy;
    double prev_vy;
    double x;
    double prev_x;
    double vx;
    double prev_vx;
} Object;

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i < numobj; i++) {
        double a_x = 0;
        double a_y = 0;
        for (int j = 0; j < numobj; j++) {
            if (j == i) {
                continue; // 自分からのクーロン力はない
            }
            a_x += objs[i].q * objs[j].q * (objs[i].x - objs[j].x) / pow(pow(objs[j].x - objs[i].x, 2) + pow(objs[j].y - objs[i].y, 2), 1.5);
            a_y += objs[i].q * objs[j].q * (objs[i].y - objs[j].y) / pow(pow(objs[j].x - objs[i].x, 2) + pow(objs[j].y - objs[i].y, 2), 1.5);
        }
        a_x *= cond.k;
        a_y *= cond.k;
        if (objs[i].m == 0.0) {
            a_x = 0; a_y = 0;
        } else {
            a_x /= objs[i].m; a_y /= objs[i].m;
        }
        objs[i].prev_vx = objs[i].vx;
        objs[i].vx += a_x * cond.dt;
        objs[i].prev_vy = objs[i].vy;
        objs[i].vy += a_y * cond.dt;
    }
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i < numobj; i++) {
        objs[i].prev_x = objs[i].x;
        objs[i].prev_y = objs[i].y;
        objs[i].x += objs[i].prev_vx * cond.dt;
        objs[i].y += objs[i].prev_vy * cond.dt;
    }
}

void my_bounce(Object objs[], const size_t numobj, const Condition cond) {
    double x_min = - cond.width/2;
    double x_max = cond.width/2;
    double y_min = - cond.height/2;
    double y_max = cond.height/2;
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
        }
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

double distance(const Object a, const Object b) {
    double d = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    return d;
}

void my_fusion(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i < numobj; i++) {
        for (int j = i + 1; j < numobj; j++) {
            // 枠内の物体の融合のみ考える！
            if (distance(objs[i], objs[j]) < 3 && objs[i].x > -cond.width/2 && objs[i].x < cond.width/2 && objs[i].y > -cond.height/2 && objs[i].y < cond.height/2) {
                objs[i].vx = (objs[i].m * objs[i].vx + objs[j].m * objs[j].vx) / (objs[i].m + objs[j].m);
                objs[i].vy = (objs[i].m * objs[i].vy + objs[j].m * objs[j].vy) / (objs[i].m + objs[j].m);
                objs[i].m = objs[i].m + objs[j].m;
                objs[i].q = objs[i].q + objs[j].q;
                objs[i].x = (objs[i].x + objs[j].x) / 2;
                objs[i].y = (objs[i].y + objs[j].y) / 2;
                objs[j].vx = 0; objs[j].vy = 0; objs[j].x = cond.width + i; objs[j].y = cond.height + j; objs[j].m = 0; objs[j].q = 0;
            }
        }
    }
}

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond) {

    printf("+");
    for (int j = 0; j < cond.width; j++) {
        printf("-");
    }
    printf("+\r\n");

    for (int i = 0; i < cond.height; i++) {
        printf("|");
        for (int j = 0; j < cond.width; j++) {
            printf(" ");
            for (int k = 0; k < numobj; k++) {
                if (cond.height/2 + objs[k].y >= i && cond.height/2 + objs[k].y < i + 1 && cond.width/2 + objs[k].x >= j && cond.width/2 + objs[k].x < j + 1) {
                    printf("\bo"); // 空白を一個消した上で物体をプロット
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

    printf("t = %lf\r\n", t);
    for (int k = 0; k < numobj; k++) {
        printf("objs[%d].m = %lf, objs[%d].q = %lf, objs[%d].x = %lf, objs[%d].y = %lf\n", k, objs[k].m, k, objs[k].q, k, objs[k].x, k, objs[k].y);
    }
    printf("\r");

}