// Driver program to test above functions.
#include "queue.h"

int main()
{
    struct Queue* queue = createQueue(1000);

    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);
    enqueue(queue, 40);

    printf("%d dequeued from queue\n\n",
           dequeue(queue));

    printf("%d dequeued from queue\n\n",
           dequeue(queue));
    return 0;
}

