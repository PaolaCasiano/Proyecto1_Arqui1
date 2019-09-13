package com.example.arqui1_p1;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;

import java.util.List;

public class imageAdapter extends BaseAdapter {
    private List<Imagen> Ids;
    private Context contexto;
    public imageAdapter(List<Imagen> Ids, Context contexto){
        this.Ids=Ids;
        this.contexto=contexto;
    }
    @Override
    public int getCount() {
        return Ids.size();
    }

    @Override
    public Object getItem(int i) {
        return null;
    }

    @Override
    public long getItemId(int i) {
        return Ids.get(i).id;
    }

    @Override
    public View getView(int i, View convertView, ViewGroup parent) {
        ImageView imageView = (ImageView) convertView;
        if(imageView==null){
            imageView = new ImageView(contexto);
            imageView.setLayoutParams(new GridView.LayoutParams(350,450));
            imageView.setScaleType(ImageView.ScaleType.CENTER_CROP);
        }
        Bitmap myBitmap = BitmapFactory.decodeFile(Ids.get(i).path);
        imageView.setImageBitmap(myBitmap);
        return imageView;
    }
}

