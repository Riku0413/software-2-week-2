# software-2-week-2

### このリポジトリについて

- 「工学部電子情報工学科 2022年度2Aセメスター講義 ソフトウェアⅡ 第2回」で作成したプログラム
- C言語により、古典力学の物理シミュレーションを実装

## 課題1
まず、以下のコードを実行する。
```
printf("\n(%p, %p, %p, %p, %p)\n", &s_end, &s3, &s2, &s1, &s_begin);
```
すると、例えば以下の出力になる。
```
(0x16d586f80, 0x16d587000, 0x16d587080, 0x16d587108, 0x16d587188)
```

続いて、以下のコードを実行する。
```
printf("\n(%zu, %zu, %zu, %zu, %zu)\n", sizeof(s_end), sizeof(s3), sizeof(s2), sizeof(s1), sizeof(s_begin));
```
すると、以下のような出力になる。
```
(128, 128, 136, 128, 128)
```
追記　パディングの状況については、struct_alignment.md に記載。

## 課題2
struct_compare.md に記載。

## 課題3
まず、構造体 Object を以下のように拡張することで、x軸方向の物理量を追加した。さらに、他の関数についてy軸方向の物理量に施していたものと全く同じ処理をx軸方向にも施すようにした。
```
typedef struct object {
    double m;
    double y;
    double prev_y;
    double vy;
    double prev_vy;
    double x;
    double prev_x;
    double vx;
    double prev_vx;
} Object;
```

ここで、my_bounce()関数において、x軸とy軸の境界は以下のように定義した。
```
double x_min = - cond.width/2;
double x_max = cond.width/2;
double y_min = - cond.height/2;
double y_max = cond.height/2;
```

## 課題4
まず、my_bouncing2.cに以下のコードを追加し、キーボード入力で物体の個数とファイル名を渡せるようにした。
```
FILE *fp = fopen(argv[2], "r");
size_t objnum = atoi(argv[1]);
Object objects[objnum];
```

続いて、以下のコードによりdatファイル内のテキストを1行ずつ格納するchar型配列を作り、さらにヘッダー文が終わるまでwhileループを回した。
```
char s[objnum][100];
while (1) {
  fgets(s[0], 100, fp);
  if (s[0][0] != '#') {
    break;
  }
}
```

次に、以下のforループによって、各物体の物理量の情報を順番にobjectsに格納した。ここで、if文では、datファイル内の物体情報の個数よりも第一引数で指定したオブジェクトの個数が多かった場合に質量ゼロの物体を盤面外に生成する。
```
for (int i = 0; i < objnum; i++) {
  int s_p = 0;
  char t[20];
  double data_list[5];
  for (int k = 0; k < 5; k++) {
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
  objects[i].x = data_list[1];
  objects[i].vx = data_list[2];
  objects[i].y = data_list[3];
  objects[i].vy = data_list[4];

  if (fgets(s[i+1], 100, fp) == NULL) {
    for (int i2 = i + 1; i2 < objnum; i2++) {
      objects[i2].m = 0;
      objects[i2].x = cond.width + i2;
      objects[i2].vx = 0;
      objects[i2].y = cond.height + i2;
      objects[i2].vy = 0;
    }
    break;
  }
}
```

注意として、my_physics2.h 内の my_update_velocities()関数を以下のように修正することにより、質量ゼロ物体が暴走しないように調整した。
```
if (objs[i].m == 0.0) {
    a_x = 0; a_y = 0;
}
```

## 課題5
まず、my_bouncing3.cファイル内の盤面更新プログラムに、以下のように物体の融合現象を実現するmy_fusion()関数を追加した。
```
my_update_velocities(objects, objnum, cond);
my_update_positions(objects, objnum, cond);
my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる

my_fusion(objects, objnum, cond);

// 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
my_plot_objects(objects, objnum, t, cond);
```

次に、参照するmy_physics3.hファイル内において、以下のような二つの物体間の距離を計算する関数を実装した。
```
double distance(const Object a, const Object b) {
    double d = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    return d;
}
```

続いて、distance()関数を用いて、一定距離範囲内に入って複数の物体が融合するmy_fusion()関数を以下のように実装した。
ここで、二つの物体が融合する場合、片方のobjsには融合後の物体、もう一方には質量ゼロの物体の情報を格納している。
```
void my_fusion(Object objs[], const size_t numobj, const Condition cond) {
    for (int i = 0; i < numobj; i++) {
        for (int j = i + 1; j < numobj; j++) {
            // 枠内の物体の融合のみ考える！
            if (distance(objs[i], objs[j]) < 3 && objs[i].x > -cond.width/2 && objs[i].x < cond.width/2 && objs[i].y > -cond.height/2 && objs[i].y < cond.height/2) {
                objs[i].vx = (objs[i].m * objs[i].vx + objs[j].m * objs[j].vx) / (objs[i].m + objs[j].m);
                objs[i].vy = (objs[i].m * objs[i].vy + objs[j].m * objs[j].vy) / (objs[i].m + objs[j].m);
                objs[i].m = objs[i].m + objs[j].m;
                objs[i].x = (objs[i].x + objs[j].x) / 2;
                objs[i].y = (objs[i].y + objs[j].y) / 2;
                objs[j].vx = 0; objs[j].vy = 0; objs[j].x = cond.width + i; objs[j].y = cond.height + j; objs[j].m = 0;
            }
        }
    }
}
```

## 課題6

### 変更点
・万有引力をクーロン力に変更
・新たに電荷量を追加
・質量を極端に大きくすることで点電荷を表現

### プログラム内の変更点

初めに、以下のように万有引力定数をクーロン定数に変更した。
```
typedef struct condition {
    int width;
    int height;
    double k; // クーロン定数
    double dt;
    double cor;
} Condition;
```

次に、my_physics4.hにおいて、下記のように電荷量qを追加した。
```
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
```

続いて、加速度の更新方法を以下のように変更し、クーロン力を実装した。
```
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
```

さらに、my_fusion()関数内に以下のコードを追加し、衝突の際に電荷量保存の法則が成り立つようにした。
```
objs[i].q = objs[i].q + objs[j].q;
objs[j].q = 0;
```

### 実行例

以上の変更点を踏まえて、プログラムを実行するにはまず次のように階層を移動する。
```
cd ./simulation
```

続いて、例えばM1のMacbookの場合、以下のコードでシンボリックリンクを作る。
```
ln -s libbounce-apple-arm64.a libbounce.a
```

次に、以下のコードによりコンパイルする。
```
gcc -o my_bouncing4 -Wall my_bouncing4.c -L. -lbounce -lm
```

最後に添付のdata3.datを引数に渡すことで以下のように実行するとプログラムが動く。
```
./my_bouncing4 3 data3.dat
```
