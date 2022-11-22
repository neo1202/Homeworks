#include <iostream>
using namespace std;
struct Building
{
    int left;
    int ht;
    int right;
};
// A strip in skyline
class Strip
{
    int left;
    int ht;

public:
    Strip(int l = 0, int h = 0)
    {
        left = l;
        ht = h;
    }
    friend class SkyLine;
};
class SkyLine
{
    // Array of strips
    Strip *arr;
    // Capacity of strip array
    int capacity;
    // Actual number of strips in array
    int n;

public:
    ~SkyLine() { delete[] arr; }
    int count() { return n; }
    // A function to merge another skylineto this skyline
    SkyLine *Merge(SkyLine *other);
    // Constructor
    SkyLine(int cap)
    {
        capacity = cap;
        arr = new Strip[cap];
        n = 0;
    }
    // Function to add a strip 'st' to array
    void append(Strip *st)
    {
        // Check for redundant strip
        if (n > 0 && arr[n - 1].ht == st->ht)
            return;
        if (n > 0 && arr[n - 1].left == st->left)
        {
            arr[n - 1].ht = max(arr[n - 1].ht, st->ht);
            return;
        }
        arr[n] = *st;
        n++;
    }
    void print()
    {
        cout << "(";
        for (int i = 0; i < n - 1; i++)
        {
            cout << arr[i].left << ","
                 << arr[i].ht << ",";
        }
        cout << arr[n - 1].left;
        cout << ")";
    }
};
// This function returns skyline for a
// given array of buildings arr[l..h].
// This function is similar to mergeSort().
SkyLine *findSkyline(Building arr[], int l, int h)
{
    if (l == h)
    {
        SkyLine *res = new SkyLine(2);
        res->append(
            new Strip(
                arr[l].left, arr[l].ht));
        res->append(
            new Strip(
                arr[l].right, 0));
        return res;
    }
    int mid = (l + h) / 2;
    SkyLine *sl = findSkyline(
        arr, l, mid);
    SkyLine *sr = findSkyline(
        arr, mid + 1, h);
    SkyLine *res = sl->Merge(sr);
    delete sl;
    delete sr;
    return res;
}
// This function merges another skyline
// 'other' to the skyline for which it is called.
// The function returns pointer to the
// resultant skyline
SkyLine *SkyLine::Merge(SkyLine *other)
{
    // Create a resultant skyline with capacity as sum of two skylines
    SkyLine *res = new SkyLine(
        this->n + other->n);
    int h1 = 0, h2 = 0;
    // Indexes of strips in two skylines
    int i = 0, j = 0;
    while (i < this->n && j < other->n)
    {
        if (this->arr[i].left < other->arr[j].left)
        {
            int x1 = this->arr[i].left;
            h1 = this->arr[i].ht;
            int maxh = max(h1, h2);
            res->append(new Strip(x1, maxh));
            i++;
        }
        else
        {
            int x2 = other->arr[j].left;
            h2 = other->arr[j].ht;
            int maxh = max(h1, h2);
            res->append(new Strip(x2, maxh));
            j++;
        }
    }
    // If there are strips left in this skyline or other skyline
    while (i < this->n)
    {
        res->append(&arr[i]);
        i++;
    }
    while (j < other->n)
    {
        res->append(&other->arr[j]);
        j++;
    }
    return res;
}
int main()
{

    Building arr[] = {
        {1, 11, 5}, {2, 6, 7}, {3, 13, 9}, {12, 7, 16}, {14, 3, 25}, {19, 18, 22}, {23, 13, 29}, {24, 4, 28}};
    int n = sizeof(arr) / sizeof(arr[0]);
    //cout<<arr[3]<<endl;
    // Find skyline for given buildings
    // and print the skyline
    SkyLine *ptr = findSkyline(arr, 0, n - 1);
    cout << " Skyline for given buildings is \n";
    ptr->print();
    return 0;
}
