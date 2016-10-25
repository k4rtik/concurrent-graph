#include <cassert> // for assert
#include <cstdlib> // for EXIT_FAILURE
#include <fstream> // for size_t, ifstream
#include <iostream>
#include <pthread.h> // for pthread_t, pthread_create, pthread_join
#include <sstream>
#include <string>   // for string
#include <unistd.h> // for usleep

#include "contraption.hpp" // for Graph, ::NTHREADS, graph_size, delete_vertex
#include "minunit.h"       // for mu_run_test

using namespace std;

int tests_run = 0;
enum { MAXGRAPHS = 11 };
Graph graphs[MAXGRAPHS];

static const char *input_file(Graph *g, string file) {
    ifstream input(file);
    if (!input.is_open()) {
        cerr << "Could not open test file for reading\n";
        return (const char *)EXIT_FAILURE;
    }

    string line;
    while (getline(input, line)) {
        stringstream linestream(line);
        ID v1, v2;
        linestream >> v1 >> v2;
        assert(v1 && v2); // zero's not supported
        insert_edge(g, v1, v2);
    }
    return 0;
}

Graph *multigraph; // for multithreaded test
pthread_key_t tid_key;

static void initialize_multigraph() {
    multigraph = new Graph;
    init_graph(multigraph);
}

void multiinsert(size_t i) {
    pthread_setspecific(tid_key, (const void *)i);
    vector<string> files = {
        "xaa", "xab", "xac", "xad", "xae", "xaf", "xag", "xah", "xai", "xaj",
        "xak", "xal", "xam", "xan", "xao", "xap", "xaq", "xar", "xas", "xat",
        "xau", "xav", "xaw", "xax", "xay", "xaz", "xba", "xbb", "xbc", "xbd",
        "xbe", "xbf", "xbg", "xbh", "xbi", "xbj", "xbk", "xbl", "xbm"};

    input_file(multigraph, files[i]);

    // TODO need better way to exit for worker threads
    while (true) {
        thread_end_loop(multigraph, i);
    }
}

static void periodic_snapshot(unsigned useconds) {
    size_t edge_count_old = 0;
    size_t edge_count_new = 0;
    while (true) {
        usleep(useconds);
        cout << "Taking Snapshot" << endl;
        edge_count_new = dump_snapshot(multigraph);
        assert(edge_count_new);
        if (edge_count_old == edge_count_new) {
            // snapshot stabilized, we can exit
            break;
        }
        edge_count_old = edge_count_new;
    }
    cout << "Total edges in final snapshot: " << edge_count_new << endl;
}

static const char *test_multi_thread_insert() {
    const int NWTHREADS = NTHREADS - 1; // number of worker threads
    static pthread_t snapshot_thread, T[NWTHREADS];
    pthread_key_create(&tid_key, 0);

    for (size_t i = 0; i < NWTHREADS; i++) {
        assert(0 == pthread_create(&T[i], 0, (void *(*)(void *))multiinsert,
                                   (void *)i));
        assert(0 == pthread_detach(T[i]));
    }

    assert(0 == pthread_create(&snapshot_thread, 0,
                               (void *(*)(void *))periodic_snapshot,
                               (void *)5000));

    assert(0 == pthread_join(snapshot_thread, 0));
    // TODO workaround for clean exit until I find a way to exit looping threads
    exit(0);

    return 0;
}

static const char *all_tests() {
    initialize_multigraph();
    mu_run_test(test_multi_thread_insert);
    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc != 1) {
        cerr << argv[0] << ": no arguments supported!\n";
        return EXIT_FAILURE;
    }

    const char *result = all_tests();
    if (result != 0) {
        cout << result << endl;
    } else {
        cout << "\nALL TESTS PASSED\n";
    }
    cout << "Tests run: " << tests_run << endl;

    return result != 0;
}
