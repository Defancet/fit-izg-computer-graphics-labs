/*!
 * @file
 * @brief This file contains timer class
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<chrono>
#include<cassert>

/**
 * @brief This class represents a timer
 *
 * @tparam TYPE type of timer
 */
template<typename TYPE>
class Timer{
  public:
    /**
     * @brief Construcor of timer
     */
    Timer(){
      reset();
    }
    /**
     * @brief This function resets the timer.
     */
    void reset(){
      start = std::chrono::high_resolution_clock::now();
      last  = start;
    }
    /**
     * @brief This function returns time elapsed from the construction of the timer.
     *
     * @return time elapsed from start
     */
    TYPE elapsedFromStart(){
      auto const newTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<TYPE>const elapsed = newTime - start;
      last = newTime;
      return elapsed.count();
    }
    /**
     * @brief This function returns time elapsed from the last time this function was called.
     *
     * @return 
     */
    TYPE elapsedFromLast(){
      auto const newTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<TYPE>const elapsed = newTime - last;
      last = newTime;
      return elapsed.count();
    }
  protected:
    /**
     * @brief Timepoint of timer construction
     */
    std::chrono::time_point<std::chrono::high_resolution_clock>start;
    /**
     * @brief Timepoint of the last elapsedFromLast call
     */
    std::chrono::time_point<std::chrono::high_resolution_clock>last ;
};
