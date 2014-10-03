#ifndef SAVAGE_CLOCK_HPP
#define SAVAGE_CLOCK_HPP
//20140726
#include<chrono>
namespace savage {
	namespace clocks {
		template<typename Duration>
		class basic_clock {
		public:
			typedef Duration duration_type;
			typedef std::chrono::time_point<std::chrono::high_resolution_clock> 
				time_point_type;
		
			explicit basic_clock(float scale=1.0) : 
					amount_(0), delta_amount_(0), scale_(scale), is_paused_(false) {
				last_update_time_ = now();
			}

			void init() {
				last_update_time_ = now();
			}

			void update() {
				if(is_paused_) { return; }
				const auto n = now();
				const auto delta = n-last_update_time_;
				delta_amount_ = std::chrono::duration_cast<duration_type>(scale_*delta);
				amount_ += std::chrono::duration_cast<duration_type>(scale_*delta);
				last_update_time_ = n;
			}

			duration_type current_time() const {
				return amount_;
			}

			duration_type delta_time() const {
				return delta_amount_;
			}



			void pause() { is_paused_ = true; }
			void resume() { 
				is_paused_ = false;
				last_update_time_ = now();
			}

			bool is_paused() const { return is_paused_; } 

			void scale(float scale) { scale_ = scale; }
			float scale() { return scale_; }

		private:
			static time_point_type now() {
				return std::chrono::high_resolution_clock::now();
			}

			std::chrono::time_point<std::chrono::high_resolution_clock> last_update_time_;
			duration_type amount_, delta_amount_;
			float scale_;
			bool is_paused_;
		};
	}
	using savage::clocks::basic_clock;
	typedef savage::basic_clock<std::chrono::microseconds> clock;

	double delta_second(savage::clock const& clock) {
		return clock.delta_time().count()*0.001*0.001;
	}
}// namespace savage

#endif //SAVAGE_CLOCK_HPP
