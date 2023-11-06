#ifndef _LOG_H____
#define _LOG_H____

extern int log_level;
#define log_level_USAGE 0
#define log_level_FATAL 1
#define log_level_BUG 1
#define log_level_ERROR 2
#define log_level_WARNING 2
#define log_level_INFO 2
#define log_level_DEBUG 3
#define log_level_TRACING 4

extern void log_set_level(int);
extern void log_inc_level(int);
extern void log_dec_level(int);
extern void log_set_name(const char *);
extern void log_set_time(int);
extern void log_set_limit(long long);
extern void log_set_ip(const char *);
extern void log_set_id(const char *);
extern void log_set_hexid(const unsigned char *, long long);
extern const char *log_get_id(void);

extern char *log_num(long long);
extern char *log_num0(long long, long long);
extern char *log_ip(unsigned char *);
extern char *log_port(unsigned char *);
extern char *log_hex(unsigned char *, long long);
extern char *log_argv(char **);

extern void log_9_(int, int, const char *, unsigned long long, const char *,
                   const char *, const char *, const char *, const char *,
                   const char *, const char *, const char *, const char *);

/* usage */
#define log_u(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_USAGE) break;                                \
        log_9_(log_level_USAGE, 0, a, b, c, d, e, f, g, h, i, j, k);           \
    } while (0)
#define log_u9(a, b, c, d, e, f, g, h, i)                                      \
    log_u(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_u8(a, b, c, d, e, f, g, h) log_u9(a, b, c, d, e, f, g, h, 0)
#define log_u7(a, b, c, d, e, f, g) log_u8(a, b, c, d, e, f, g, 0)
#define log_u6(a, b, c, d, e, f) log_u7(a, b, c, d, e, f, 0)
#define log_u5(a, b, c, d, e) log_u6(a, b, c, d, e, 0)
#define log_u4(a, b, c, d) log_u5(a, b, c, d, 0)
#define log_u3(a, b, c) log_u4(a, b, c, 0)
#define log_u2(a, b) log_u3(a, b, 0)
#define log_u1(a) log_u2(a, 0)

/* bug */
#define log_b(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_BUG) break;                                  \
        log_9_(log_level_BUG, 2, a, b, c, d, e, f, g, h, i, j, k);             \
    } while (0)
#define log_b9(a, b, c, d, e, f, g, h, i)                                      \
    log_b(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_b8(a, b, c, d, e, f, g, h) log_b9(a, b, c, d, e, f, g, h, 0)
#define log_b7(a, b, c, d, e, f, g) log_b8(a, b, c, d, e, f, g, 0)
#define log_b6(a, b, c, d, e, f) log_b7(a, b, c, d, e, f, 0)
#define log_b5(a, b, c, d, e) log_b6(a, b, c, d, e, 0)
#define log_b4(a, b, c, d) log_b5(a, b, c, d, 0)
#define log_b3(a, b, c) log_b4(a, b, c, 0)
#define log_b2(a, b) log_b3(a, b, 0)
#define log_b1(a) log_b2(a, 0)

/* fatal */
#define log_f(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_FATAL) break;                                \
        log_9_(log_level_FATAL, 1, a, b, c, d, e, f, g, h, i, j, k);           \
    } while (0)
#define log_f9(a, b, c, d, e, f, g, h, i)                                      \
    log_f(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_f8(a, b, c, d, e, f, g, h) log_f9(a, b, c, d, e, f, g, h, 0)
#define log_f7(a, b, c, d, e, f, g) log_f8(a, b, c, d, e, f, g, 0)
#define log_f6(a, b, c, d, e, f) log_f7(a, b, c, d, e, f, 0)
#define log_f5(a, b, c, d, e) log_f6(a, b, c, d, e, 0)
#define log_f4(a, b, c, d) log_f5(a, b, c, d, 0)
#define log_f3(a, b, c) log_f4(a, b, c, 0)
#define log_f2(a, b) log_f3(a, b, 0)
#define log_f1(a) log_f2(a, 0)

/* error */
#define log_e(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_ERROR) break;                                \
        log_9_(log_level_ERROR, 1, a, b, c, d, e, f, g, h, i, j, k);           \
    } while (0)
#define log_e9(a, b, c, d, e, f, g, h, i)                                      \
    log_e(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_e8(a, b, c, d, e, f, g, h) log_e9(a, b, c, d, e, f, g, h, 0)
#define log_e7(a, b, c, d, e, f, g) log_e8(a, b, c, d, e, f, g, 0)
#define log_e6(a, b, c, d, e, f) log_e7(a, b, c, d, e, f, 0)
#define log_e5(a, b, c, d, e) log_e6(a, b, c, d, e, 0)
#define log_e4(a, b, c, d) log_e5(a, b, c, d, 0)
#define log_e3(a, b, c) log_e4(a, b, c, 0)
#define log_e2(a, b) log_e3(a, b, 0)
#define log_e1(a) log_e2(a, 0)

/* warning */
#define log_w(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_WARNING) break;                              \
        log_9_(log_level_WARNING, 2, a, b, c, d, e, f, g, h, i, j, k);         \
    } while (0)
#define log_w9(a, b, c, d, e, f, g, h, i)                                      \
    log_w(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_w8(a, b, c, d, e, f, g, h) log_w9(a, b, c, d, e, f, g, h, 0)
#define log_w7(a, b, c, d, e, f, g) log_w8(a, b, c, d, e, f, g, 0)
#define log_w6(a, b, c, d, e, f) log_w7(a, b, c, d, e, f, 0)
#define log_w5(a, b, c, d, e) log_w6(a, b, c, d, e, 0)
#define log_w4(a, b, c, d) log_w5(a, b, c, d, 0)
#define log_w3(a, b, c) log_w4(a, b, c, 0)
#define log_w2(a, b) log_w3(a, b, 0)
#define log_w1(a) log_w2(a, 0)

/* info */
#define log_i(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_INFO) break;                                 \
        log_9_(log_level_INFO, 0, a, b, c, d, e, f, g, h, i, j, k);            \
    } while (0)
#define log_i9(a, b, c, d, e, f, g, h, i)                                      \
    log_i(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_i8(a, b, c, d, e, f, g, h) log_i9(a, b, c, d, e, f, g, h, 0)
#define log_i7(a, b, c, d, e, f, g) log_i8(a, b, c, d, e, f, g, 0)
#define log_i6(a, b, c, d, e, f) log_i7(a, b, c, d, e, f, 0)
#define log_i5(a, b, c, d, e) log_i6(a, b, c, d, e, 0)
#define log_i4(a, b, c, d) log_i5(a, b, c, d, 0)
#define log_i3(a, b, c) log_i4(a, b, c, 0)
#define log_i2(a, b) log_i3(a, b, 0)
#define log_i1(a) log_i2(a, 0)

/* debug */
#define log_d(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_DEBUG) break;                                \
        log_9_(log_level_DEBUG, 1, a, b, c, d, e, f, g, h, i, j, k);           \
    } while (0)
#define log_d9(a, b, c, d, e, f, g, h, i)                                      \
    log_d(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_d8(a, b, c, d, e, f, g, h) log_d9(a, b, c, d, e, f, g, h, 0)
#define log_d7(a, b, c, d, e, f, g) log_d8(a, b, c, d, e, f, g, 0)
#define log_d6(a, b, c, d, e, f) log_d7(a, b, c, d, e, f, 0)
#define log_d5(a, b, c, d, e) log_d6(a, b, c, d, e, 0)
#define log_d4(a, b, c, d) log_d5(a, b, c, d, 0)
#define log_d3(a, b, c) log_d4(a, b, c, 0)
#define log_d2(a, b) log_d3(a, b, 0)
#define log_d1(a) log_d2(a, 0)

/* tracing */
#define log_t(a, b, c, d, e, f, g, h, i, j, k)                                 \
    do {                                                                       \
        if (log_level < log_level_TRACING) break;                              \
        log_9_(log_level_TRACING, 1, a, b, c, d, e, f, g, h, i, j, k);         \
    } while (0)
#define log_t9(a, b, c, d, e, f, g, h, i)                                      \
    log_t(__FILE__, __LINE__, a, b, c, d, e, f, g, h, i)
#define log_t8(a, b, c, d, e, f, g, h) log_t9(a, b, c, d, e, f, g, h, 0)
#define log_t7(a, b, c, d, e, f, g) log_t8(a, b, c, d, e, f, g, 0)
#define log_t6(a, b, c, d, e, f) log_t7(a, b, c, d, e, f, 0)
#define log_t5(a, b, c, d, e) log_t6(a, b, c, d, e, 0)
#define log_t4(a, b, c, d) log_t5(a, b, c, d, 0)
#define log_t3(a, b, c) log_t4(a, b, c, 0)
#define log_t2(a, b) log_t3(a, b, 0)
#define log_t1(a) log_t2(a, 0)

#endif
