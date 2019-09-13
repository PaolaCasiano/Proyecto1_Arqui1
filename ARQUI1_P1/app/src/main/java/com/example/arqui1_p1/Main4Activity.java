package com.example.arqui1_p1;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.GridLayout;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.ArrayList;
import java.util.LinkedList;


public class Main4Activity extends Activity  {
    Button btnEliminar;
    ArrayList<Imagen> f = new ArrayList<>();
    File[] listFile;
    String pathtemp;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main4);
        getImagenes();
        GridView gridView = findViewById(R.id.myGrid);
        gridView.setAdapter(new imageAdapter(f,this));
        btnEliminar = (Button)findViewById(R.id.btn_eliminar);
        btnEliminar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                File myFile = new File(pathtemp);
                if(myFile.exists())
                    myFile.delete();
                Toast.makeText(getApplicationContext(),"Imagen Eliminada Exitosamente",Toast.LENGTH_SHORT).show();
                finish();
                startActivity(getIntent());
            }
        });
        gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                int posactual=f.get(i).id;
                Log.e("POSICION","La posicion es: "+posactual);
                pathtemp=f.get(posactual).path;
            }
        });
    }
    public void getImagenes()
    {
        File file= new File(android.os.Environment.getExternalStorageDirectory(),"imagenespaint");

        if (file.isDirectory())
        {
            listFile = file.listFiles();


            for (int i = 0; i < listFile.length; i++)
            {

                f.add(new Imagen(i,listFile[i].getAbsolutePath()));

            }
        }
    }

}
