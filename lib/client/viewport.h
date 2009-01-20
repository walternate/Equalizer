
/* Copyright (c) 2006-2009, Stefan Eilemann <eile@equalizergraphics.com> 
   All rights reserved. */

#ifndef EQ_VIEWPORT_H
#define EQ_VIEWPORT_H

#include <eq/base/base.h>

#include <iostream>

namespace eq
{
    /**
     * Holds a fractional viewport along with some methods for manipulation.
     */
    class Viewport 
    {
    public:
        /**
         * @name Constructors
         */
        //*{
        Viewport() : x(0.0f), y(0.0f), w(1.0f), h(1.0f)  {}

        Viewport( const float x_, const float y_, const float w_,const float h_)
                : x(x_), y(y_), w(w_), h(h_)  {}
        //*}

        void invalidate() { x=0.0f; y=0.0f; w=-1.0f; h=-1.0f; }
        void apply ( const Viewport& rhs )
            {
                x += rhs.x * w;
                y += rhs.y * h;
                w *= rhs.w;
                h *= rhs.h;
            }

        bool operator == ( const Viewport& rhs ) const 
            { 
                return ( x==rhs.x && y==rhs.y && w==rhs.w && h==rhs.h);
            }

        bool operator != ( const Viewport& rhs ) const 
            { 
                return ( x!=rhs.x || x!=rhs.y || w!=rhs.w || h!=rhs.h);
            }

        /** 
         * @return true if the viewport has a non-negative, but potentially
         *         empty, size.
         */
        bool isValid() const { return (w>=0.0f && h>=0.0f); }
        
        /** 
         * @return true if the viewport has a non-zero area, i.e, it is
         *         not empty.
         */
        bool hasArea() const { return (w>0.0f && h>0.0f); }

        /** @return the area of this viewport */
        float getArea() const { return w*h; }

        /** @return the X end position */
        float getXEnd() const { return x+w; }

        /** @return the Y end position */
        float getYEnd() const { return y+h; }

        /** create the intersection viewport  */
        void intersect( const Viewport& rhs )
            {
                if( *this == rhs )
                    return;

                if( !rhs.isValid() || !isValid() )
                {
                    invalidate();
                    return;
                }
                
                if( !rhs.hasArea() || !hasArea() )
                {
                    x = 0;
                    y = 0;
                    w = 0;
                    h = 0;
                    return;
                }
                
                const int32_t sEx =     x +     w;
                const int32_t sEy =     y +     h;
                const int32_t dEx = rhs.x + rhs.w;
                const int32_t dEy = rhs.y + rhs.h;
                    
                x = EQ_MAX( x, rhs.x );
                y = EQ_MAX( y, rhs.y );
                w = EQ_MIN( sEx, dEx ) - x;
                h = EQ_MIN( sEy, dEy ) - y;
            }

        float x;
        float y;
        float w;
        float h;

        EQ_EXPORT static const Viewport FULL;
    };

    inline std::ostream& operator << ( std::ostream& os, const Viewport& vp )
    {
        os << "[ " << vp.x << " " << vp.y << " " << vp.w << " " << vp.h << " ]";
        return os;
    }
}

#endif // EQ_VIEWPORT_H
