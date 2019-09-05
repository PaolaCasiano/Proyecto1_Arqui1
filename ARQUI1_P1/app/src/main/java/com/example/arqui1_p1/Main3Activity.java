package com.example.arqui1_p1;

import android.Manifest;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;

import com.github.clans.fab.FloatingActionMenu;
import com.github.clans.fab.FloatingActionButton;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Locale;
import java.util.UUID;

public class Main3Activity extends AppCompatActivity {
    final int MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE=45;//
    final int MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE=46;

    String dato;
    TextView txt_nombre;
    Lienzo lienzo;

    FloatingActionMenu fbMenu;
    FloatingActionButton fbNegro;
    FloatingActionButton fbAzul;
    FloatingActionButton fbRojo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main3);

        BottomNavigationView navView = findViewById(R.id.nav_view);
        navView.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);

        lienzo = findViewById(R.id.view);

        dato = getIntent().getStringExtra("dato");

        txt_nombre = findViewById(R.id.txt_nombre);
        txt_nombre.setText(dato);


        fbMenu = (FloatingActionMenu) findViewById(R.id.floatingActionMenu);
        fbNegro= (FloatingActionButton) findViewById(R.id.floatingActionButtonNegro);
        fbAzul= (FloatingActionButton) findViewById(R.id.floatingActionButtonAzul);
        fbRojo= (FloatingActionButton) findViewById(R.id.floatingActionButtonRojo);

        fbNegro.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                lienzo.setColor(Color.BLACK);
            }
        });
        fbAzul.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                lienzo.setColor(Color.BLUE);
            }
        });
        fbRojo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                lienzo.setColor(Color.RED);
            }
        });


    }

    @Override
    public void onBackPressed() {
        try {
            Intent i = new Intent(this,MainActivity.class);
            startActivity(i);
        }catch (Error e)
        {
            super.onBackPressed();
        }

    }
    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            switch (item.getItemId()) {
                case R.id.navigation_lapiz:
                    lienzo.activarPen(1);
                    lienzo.setColor(Color.BLACK);
                    return true;
                case R.id.navigation_borrador:
                    lienzo.activarPen(2);
                    lienzo.setBorrado();
                    return true;
                case R.id.navigation_imprimir:
                    lienzo.activarPen(0);
                    dialogoGuardar();
                    //Metodo para guardar la imagen y mandar a imprimir
                    return true;
                case R.id.navigation_nuevo:
                    lienzo.activarPen(0);
                    lienzo.nuevoLienzo();
                    return true;
            }
            return false;
        }
    };
    protected void dialogoGuardar() {
        AlertDialog.Builder salvarDibujo = new AlertDialog.Builder(this);
        salvarDibujo.setTitle("Guardar dibujo");
        salvarDibujo.setMessage("¿Guardar el dibujo en la galería?");
        salvarDibujo.setPositiveButton("Aceptar", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                solicitarPermisos();

                String file_path = Environment.getExternalStorageDirectory().getAbsolutePath() +
                        "/imagenesPaint";
                File dir = new File(file_path);
                if(!dir.exists())
                    dir.mkdirs();
                File file = new File(dir,  dato + ".png");

                if(file.exists()){
                    boolean result = file.delete();
                    if(result)
                        Log.v("Msj","Se elimino el archivo");
                    else
                        Log.v("Msj","No se elimino el archivo");
                }

                FileOutputStream fOut = null;
                //lienzo.setDrawingCacheEnabled(true);
                try {
                    fOut = new FileOutputStream(file);
                    lienzo.canvasBitmap.compress(Bitmap.CompressFormat.PNG, 100, fOut);
                    fOut.flush();
                    fOut.close();
                } catch (FileNotFoundException e) {
                    Log.e("Buscar","Exception por archivo no encontrado");

                } catch (IOException e) {
                    Log.e("Buscar","Exception por flush y close");
                }


//                lienzo.setDrawingCacheEnabled(true);
//                String imgSv = MediaStore.Images.Media.insertImage(
//                        getContentResolver(), lienzo.getDrawingCache(),
//                        dato + ".png", "drawing"
//                );
//                if (imgSv != null) {
//                  Toast savedToast = Toast.makeText(getApplicationContext(), "Imagen guardada con éxito!", Toast.LENGTH_SHORT);
//                    savedToast.show();
//                } else {
//                  Toast unsavedToast = Toast.makeText(getApplicationContext(), "Error, la imagen no fue guardada!", Toast.LENGTH_SHORT);
//                    unsavedToast.show();
//                }
            }
        });
        salvarDibujo.setNegativeButton("Cancelar", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                dialogInterface.cancel();
            }
        });
        salvarDibujo.show();
    }
    protected void solicitarPermisos(){
        if(ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
            // Permission is not granted
            // Should we show an explanation?
//            if(ActivityCompat.shouldShowRequestPermissionRationale(this,
//                    Manifest.permission.WRITE_EXTERNAL_STORAGE)){
//                // Show an explanation to the user *asynchronously* -- don't block
//                // this thread waiting for the user's response! After the user
//                // sees the explanation, try again to request the permission.
//            }
//            else{
            // No explanation needed; request the permission
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE);
            // MY_PERMISSIONS_REQUEST_READ_CONTACTS is an
            // app-defined int constant. The callback method gets the
            // result of the request.
//            }

        }
        else {
            // Permission has already been granted
        }
        if(ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
            // Permission is not granted
            // Should we show an explanation?
//            if(ActivityCompat.shouldShowRequestPermissionRationale(this,
//                    Manifest.permission.WRITE_EXTERNAL_STORAGE)){
//                // Show an explanation to the user *asynchronously* -- don't block
//                // this thread waiting for the user's response! After the user
//                // sees the explanation, try again to request the permission.
//            }
//            else{
            // No explanation needed; request the permission
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE);
            // MY_PERMISSIONS_REQUEST_READ_CONTACTS is an
            // app-defined int constant. The callback method gets the
            // result of the request.
//            }

        }
        else {
            // Permission has already been granted
        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String[] permissions, int[] grantResults) {
        switch (requestCode) {
            case MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // permission was granted, yay! Do the
                    // contacts-related task you need to do.
                    Log.v("Msj","Permission is granted");
                } else {
                    Log.v("Msj","Permission denied");
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                }
                return;
            }
            case MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE:
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // permission was granted, yay! Do the
                    // contacts-related task you need to do.
                    Log.v("Msj","Permission is granted");
                } else {
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                    Log.v("Msj","Permission denied");
                }
                return;
            // other 'case' lines to check for other
            // permissions this app might request.
        }
    }
}
