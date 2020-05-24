
#include "stdafx.h"

static bool is_ok(int p, int q, int N)
{
    return
        0 <= p && p <= N &&
        0 <= q && q <= N &&
        0 <= p + q && p + q <= N;
}

static int count_up(int N)
{
    int count = 0;
    for (int a = 0; a < N; a++)
    {
        for (int b = 0; b < N; b++)
        {
            if (a + b < N)
            {
                for (int n = 1; n <= N - a - b; n++)
                {
                    if (is_ok(a, b, N) &&
                        is_ok(a + n, b, N) &&
                        is_ok(a, b + n, N))
                    {
                        count += 1;
                    }
                }
            }
        }
    }
    return count;
}

static int count_down(int N)
{
    int count = 0;
    for (int b = 1; b < N; b++)
    {
        for (int e = 0; e < N; e++)
        {
            if (b + e < N)
            {
#undef min
                int nmax = std::min(N - b - e, b);
                for (int n = 1; n <= nmax; n++)
                {
                    if (is_ok(b, e, N) &&
                        is_ok(b - n, e, N) &&
                        is_ok(b, e + n, N))
                    {
                        count += 1;
                    }
                }
            }
        }
    }
    return count;
}

struct Point {
    int x;
    int y;
    Point(int a, int b) : x(a), y(b) {}
    bool allowed() const
    {
        if (x == 0 || y == 0) return true;
        if (x + y == 0) return true;
        return false;
    }
    void print() const
    {
        printf("%3d %3d\n", x, y);
    }
    bool is_ok(int N) const
    {
        return
            0 <= x && x <= N &&
            0 <= y && y <= N &&
            0 <= x + y && x + y <= N;
    }
};

static bool operator<(const Point & a, const Point & b)
{
    if (a.x < b.x) return true;
    if (a.x > b.x) return false;
    if (a.y < b.y) return true;
    if (a.y > b.y) return false;
    return false;
}

static Point operator-(const Point & a, const Point & b)
{
    return Point(a.x - b.x, a.y - b.y);
}

static int area(const Point & a, const Point & b, const Point & c)
{
    Point a1 = a - c;
    Point b1 = b - c;
    return a1.x * b1.y - a1.y * b1.x;
}

static int count_bf(int N)
{
    int count = 0;
    const int N1 = N + 1;
    for (int ix = 0; ix < N1; ix++)
    {
        for (int iy = 0; iy < N1; iy++)
        {
            const Point p(ix, iy);
            if (p.is_ok(N))
            {
                for (int n = -N; n <= N; n++)
                {
                    const Point q(p.x + n, p.y);
                    const Point r(p.x, p.y + n);
                    if (n != 0 && q.is_ok(N) && r.is_ok(N))// && area(p - q, q - r, r - p) != 0)
                    {
                        /*q.print();
                        r.print();
                        printf("\n");*/
                        count += 1;
                    }
                }
            }
        }
    }
    return count;
}

static double convert_ft_to_secs(const FILETIME & ft)
{
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return u.QuadPart * 100.0e-9;
}

static double get_cpu_time()
{
    FILETIME CreationTime = { 0 };
    FILETIME ExitTime = { 0 };
    FILETIME KernelTime = { 0 };
    FILETIME UserTime = { 0 };
    int ok = GetProcessTimes(GetCurrentProcess(), &CreationTime, &ExitTime, &KernelTime, &UserTime);
    if (ok)
        return convert_ft_to_secs(UserTime) + convert_ft_to_secs(KernelTime);
    else
        return 0;
}

static double get_wallclock_time()
{
    LARGE_INTEGER time, freq;
    if (!QueryPerformanceFrequency(&freq))
    {
        return 0;
    }
    if (!QueryPerformanceCounter(&time))
    {
        return 0;
    }
    return time.QuadPart / (double)freq.QuadPart;
}

static double print_times(double start)
{
    double wall_clock = get_wallclock_time();
    printf("cpu time = %f wall clock time %f\n", get_cpu_time(), wall_clock - start);
    return wall_clock;
}

static void calculate(int N, FILE *fp)
{
    int bf = count_bf(N);
    int up = count_up(N);
    int down = count_down(N);
    int sum = up + down;
    int poly = 2 * N * N * N + 5 * N * N + 2 * N;
    int even = poly / 8;
    int odd = (poly + 1) / 8;
    int prediction = (N % 2) ? odd : even;
    int discrepancy = sum - prediction;
    printf("N = %2d N^2 = %5d up = %3d down = %3d tot = %4d bf = %4d discrepancy = %d ",
        N, N * N, up, down, sum, bf, discrepancy);
    fprintf(fp, "%2d %5d %3d %3d %4d %4d %d\n", N, N * N, up, down, sum, bf, discrepancy);
}

static void distribute(int offset, int nr_threads, FILE *fp, double start_time)
{
    for (int N = 1 + offset; N < 500; N += nr_threads)
    {
        calculate(N, fp);
        print_times(start_time);
    }
}

int main()
{
    double start_time = print_times(0.0);
    FILE *fp = nullptr;
    fopen_s(&fp, "triangles.dat", "wt");
    if (fp)
    {
        if (true)
        {
            int nr_threads = 3;
            std::vector<std::thread> threads;
            for (int t = 0; t < nr_threads; t++)
            {
                threads.push_back(std::thread(distribute, t, nr_threads, fp, start_time));
            }
            for (int t = 0; t < (int)threads.size(); t++)
            {
                threads[t].join();
            }
        }
        else
        {
            for (int N = 1; N < 500; N++)
            {
                calculate(N, fp);
                print_times(start_time);
            }
        }
        fclose(fp);
    }

    return 0;
}

