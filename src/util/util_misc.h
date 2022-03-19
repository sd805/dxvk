#pragma once

#include <d3d9types.h>

namespace dxvk {

  /**
   * \brief Stall tracker
   */
  class StallTracker {

  public:

    /**
     * \brief Checks whether the resource is stalling
     * \returns \c true if the resource is marked as stalling
     */
    bool isStalling() const {
      return bool(m_history & 1u);
    }

    /**
     * \brief Reports a use
     */
    void notifyUse() {
      // Preserve the stall bit so that once we don't
      // demote a stalling resource to non-stallinng
      m_history = (m_history << 1u) | (m_history & 1u);
    }

    /**
     * \brief Reports a stall
     *
     * Updates the stall flag if at least half the
     * recorded uses have been marked as stalling.
     */
    void notifyStall() {
      m_history |= 2u;

      if (bit::popcnt(m_history) >= 16)
        m_history |= 1u;
    }

  private:

    uint32_t m_history = 0u;

  };

  /**
   * \brief Decodes D3D9 color
   *
   * \param [in] color D3D9 color
   * \param [out] rgba Clear color
   */
  inline void DecodeD3DCOLOR(D3DCOLOR color, float* rgba) {
    // Encoded in D3DCOLOR as argb
    rgba[0] = (float)((color & 0x00ff0000) >> 16) / 255.0f;
    rgba[1] = (float)((color & 0x0000ff00) >> 8)  / 255.0f;
    rgba[2] = (float)((color & 0x000000ff))       / 255.0f;
    rgba[3] = (float)((color & 0xff000000) >> 24) / 255.0f;
  }

}