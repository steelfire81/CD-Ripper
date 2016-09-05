#pragma once

// Abstract outline for classes listening for import progress
class CDProgressListener {
	public:
		// called when song import ends
		virtual void notifyEnd() = 0;

		// called whenever sectors are read
		virtual void notifyProgress(double progress) = 0;

		// called when song import begins
		virtual void notifyStart() = 0;
};