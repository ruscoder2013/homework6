#include "async.h"

int main(int argc, char *argv[]) {
    auto handler = async::connect(3);
    async::receive(handler, "cmd1\n", 5);
    async::receive(handler, "cmd2\n", 5);
    async::receive(handler, "cmd3\ncmd4\ncmd", 14);
    async::receive(handler, "5\n", 3);
    async::receive(handler, "cmd6\n", 5);
    async::receive(handler, "cmd7\n", 5);
    async::receive(handler, "cmd8\n", 5);
    async::disconnect(handler);
    return 0;
}