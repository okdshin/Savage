#ifndef SAVAGE_CLOCK_H
#define SAVAGE_CLOCK_H
//20140726
#include<chrono>
namespace savage {
	typedef unsigned long long uint64;
	typedef float float32;

	template<typename Duration>
	class clock {
	public:
		typedef Duration duration_type;
		typedef std::chrono::time_point<std::chrono::high_resolution_clock> 
			time_point_type;
	
		explicit clock(float32 scale=1.0) : 
				amount_(0), scale_(scale), is_paused_(false) {
			last_update_time_ = now();
		}

		void init() {
			last_update_time_ = now();
		}

		void update() {
			if(is_paused_) { return; }
			const auto n = now();
			const auto delta = n-last_update_time_;
			amount_ += std::chrono::duration_cast<duration_type>(scale_*delta);
			last_update_time_ = n;
		}

		duration_type current_time() const {
			return amount_;
		}

		void pause() { is_paused_ = true; }
		void resume() { 
			is_paused_ = false;
			last_update_time_ = now();
		}

		bool is_paused() const { return is_paused_; } 

		void scale(float32 scale) { scale_ = scale; }
		float32 scale() { return scale_; }

	private:
		static time_point_type now() {
			return std::chrono::high_resolution_clock::now();
		}

		std::chrono::time_point<std::chrono::high_resolution_clock> last_update_time_;
		duration_type amount_;
		float32 scale_;
		bool is_paused_;
	};
}// namespace savage

#endif //SAVAGE_CLOCK_H