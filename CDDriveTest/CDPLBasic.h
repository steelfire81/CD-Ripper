#pragma once

#include "stdafx.h"

#define CDPLB_INTERVAL 5

class CDPLBasic : public CDProgressListener {
	public:
		CDPLBasic();
		void notifyEnd();
		void notifyProgress(double progress);
		void notifyStart();
	private:
		int previousUpdate;
};