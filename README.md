# Hobby-coding

## crashing

online link: https://www.jdoodle.com/a/1FFm


input:

số_tháng_cần_crash số_lượng_cv

tên_cv ET_bt C_bt ET_gấp C_gấp danh_sách_cv_phụ_thuộc


VD:
Mỗi công việc sau được mô tả là: Tên(ET bt, C bt, ET gấp, C gấp, Phụ thuộc), thời gian : tháng, tiền : usd 
A(2, 10, 1, 16, -), B(3, 20, 1, 38, A), C(2, 10, 1, 18, B), D(5, 40, 3, 80, -), E(5, 10, 2, 19, -), F(5, 30, 2, 45, C)
Hãy tìm phương án làm gấp để dự án hoàn thành trong vòng 7 tháng với tổng chi phí làm gấp là nhỏ nhất. 

Input:

```
7 6
A 2 10 1 16
B 3 20 1 38 A
C 2 10 1 18 B
D 5 40 3 80
E 5 10 2 19
F 5 30 2 45 C
```

=> output

```
+------+----------+---------+-------+---------------+
| Step |  Crashed | Cr.Cost | T.End |      Next     |
+------+----------+---------+-------+---------------+
|     0|          |        0|     12|           ABCF|
+------+----------+---------+-------+---------------+
|     1|         A|        6|     11|            BCF|
+------+----------+---------+-------+---------------+
|     2|         C|        8|     11|            ABF|
+------+----------+---------+-------+---------------+
|     3|        AC|       14|     10|             BF|
+------+----------+---------+-------+---------------+
|     4|         F|       15|      9|            ABC|
+------+----------+---------+-------+---------------+
|     5|         B|       18|     10|            ACF|
+------+----------+---------+-------+---------------+
|     6|        AF|       21|      8|             BC|
+------+----------+---------+-------+---------------+
|     7|        CF|       23|      8|             AB|
+------+----------+---------+-------+---------------+
|     8|        AB|       24|      9|             CF|
+------+----------+---------+-------+---------------+
|     9|        BC|       26|      9|             AF|
+------+----------+---------+-------+---------------+
|    10|       ACF|       29|      7|              B|
+------+----------+---------+-------+---------------+

```
