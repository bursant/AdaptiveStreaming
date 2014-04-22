#include "Downloader.h"

int Downloader::IORead(uint8_t *buf, int buf_size)
{
	IBufferItem *item = this->buffer->front();

    if(item == NULL)
        return 0;

    int ret = item->read(buf, buf_size);

    if(ret == 0)
        this->buffer->pop();
    else
        return ret;

    return this->IORead(buf, buf_size);
}

Downloader::Downloader(IBuffer* buffer, IMPD* mpd, IDASHManager* manager, IAdaptation* adaptation)
{
	this->buffer = buffer;
	this->mpd = mpd;
	this->manager = manager;
	this->adaptation = adaptation;

    this->bufferingThread = CreateThreadPortable (bufferingProcedure, this);
}

Downloader::~Downloader()
{
	delete(manager);
}

void* Downloader::bufferingProcedure(void *receiver)
{
    Downloader* downloader = (Downloader *) receiver;

    uint32_t number = 0;

    IBufferItem *media = downloader->adaptation->getSegment(number);

    while(media != NULL)
    {
        media->start();
        media->wait();
        downloader->buffer->push(media);
        number++;

        media = downloader->adaptation->getSegment(number);

        //std::cout << "Buffer size in segments: " << downloader->buffer->size() << std::endl;
    }

    downloader->buffer->setEnd(true);
    return NULL;
}