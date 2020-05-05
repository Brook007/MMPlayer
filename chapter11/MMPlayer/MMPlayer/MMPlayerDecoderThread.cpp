#include "MMPlayer.h"

MMPlayerDecoderThread::MMPlayerDecoderThread()
{
	decoder = new MMAVDecoder();
}

MMPlayerDecoderThread::~MMPlayerDecoderThread()
{
	if (decoder != nullptr) {
		delete decoder;
		decoder = nullptr;
	}
}

int MMPlayerDecoderThread::Init(MMAVStream* avStream)
{
	return decoder->Init(avStream);
}

int MMPlayerDecoderThread::PutPacket(MMAVPacket* pkt)
{
	return packetQueue.Push(pkt);
}

int MMPlayerDecoderThread::GetPacketQueueSize()
{
	return packetQueue.Size();
}

void MMPlayerDecoderThread::run()
{
	int frameCount = 0;
	// ���н������
	while (!stopFlag) {
		MMAVPacket* pkt = nullptr;
		int ret = packetQueue.Pop(&pkt);
		if (ret) {
			// ����Ϊ�գ�ȡ���� Packet
			continue;
		}

		decoder->SendPacket(pkt);
		while (1) {
			MMAVFrame frame;
			ret = decoder->RecvFrame(&frame);
			if (ret) {
				break;
			}
			frameCount++;
			printf("Decoder Success: %d\n", frameCount);
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}