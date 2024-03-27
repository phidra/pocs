#include <iostream>
#include <boost/heap/fibonacci_heap.hpp>

using namespace std;

void basic_interface(void) {
    boost::heap::fibonacci_heap<int> pq;

    pq.push(2);
    pq.emplace(42);
    pq.push(1);

    cout << "Priority Queue: popped elements" << endl;
    cout << pq.top() << "  (size=" << pq.size() << " / empty=" << pq.empty() << ")" << endl;  // 3
    pq.pop();
    cout << pq.top() << "  (size=" << pq.size() << " / empty=" << pq.empty() << ")" << endl;  // 2
    pq.pop();
    cout << pq.top() << "  (size=" << pq.size() << " / empty=" << pq.empty() << ")" << endl;  // 1
    pq.pop();
    cout << "   (size=" << pq.size() << " / empty=" << pq.empty() << ")" << endl;  // 1
    cout << endl;

    // needed by dijkstra CH :
    //    - top
    //    - pop
    //    - size
    //    - empty
    //    - emplace
}

int main(int argc, char* argv[]) {
    cout << "coucou" << endl;
    basic_interface();
}
