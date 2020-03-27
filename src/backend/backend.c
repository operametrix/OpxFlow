
int subscriber_list[50] = {0};
int subscribers = 0;

void backend_add_subscriber(int fd)
{
    subscriber_list[subscribers] = fd;
    subscribers++;
}

int backend_get_subscriber(int index)
{
    return subscriber_list[index];
}

int backend_get_subscribers_size(void)
{
    return subscribers;
}