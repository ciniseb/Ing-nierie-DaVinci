package com.ingenieriedavinci.leonardusui;

import android.content.Context;
import android.util.AttributeSet;

/**
 * Created by sebastien on 18-02-28.
 */

public class ImageViewCarre extends android.support.v7.widget.AppCompatImageView {

    public ImageViewCarre(Context context) {
        super(context);
    }

    public ImageViewCarre(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ImageViewCarre(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    public void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
        super.onMeasure(widthMeasureSpec, widthMeasureSpec);
    }


}