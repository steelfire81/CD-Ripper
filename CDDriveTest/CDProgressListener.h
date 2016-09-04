#pragma once

class CDProgressListener {
	public:
		virtual void notifyProgress(double progress) = 0;
};