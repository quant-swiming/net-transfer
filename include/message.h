#pragma once 

#pragma pack(1)
enum MsgType {
	HandShake,
	Heart,
	Transfer
};

struct HandShakeMsg {
	HandShakeMsg() {
		type = MsgType::HandShaeke;
	}

	enum MsgType type;
	char token[1024];
};

#pragma pack()
