package com.example.arqui1_p1;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.TextView;


import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.io.File;
<<<<<<< HEAD
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;


public class Main4Activity extends Activity  {
    Button btnEliminar,btnImprimir;
    ArrayList<Imagen> f = new ArrayList<>();
    File[] listFile;
    String pathtemp;
    String url = "http://192.168.43.240:8000/servidorApp/buscar_imagen";
    RequestQueue rq;
    String nombre = "";
=======



public class Main4Activity extends AppCompatActivity implements View.OnClickListener {
    final int MY_PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE=45;//
    final int MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE=46;
    Button btncargar,btneliminar;
    TextView tv;
    ImageView imagen;
    String pathactual;

>>>>>>> 88c9e11fa1d68616f86450593f12f6aeeb1adc14
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        rq = Volley.newRequestQueue(this);
        setContentView(R.layout.activity_main4);
<<<<<<< HEAD
        getImagenes();
        GridView gridView = findViewById(R.id.myGrid);
        gridView.setAdapter(new imageAdapter(f,this));
        btnImprimir = (Button) findViewById(R.id.btn_imprimir);
        btnEliminar = (Button)findViewById(R.id.btn_eliminar);
        btnEliminar.setOnClickListener(new View.OnClickListener() {
=======

        tv = findViewById(R.id.textView2);
        imagen = findViewById(R.id.imageView);
        btncargar=findViewById(R.id.button5);
        btncargar.setOnClickListener(this);
        btneliminar= findViewById(R.id.button6);
        btneliminar.setOnClickListener(new View.OnClickListener() {
>>>>>>> 88c9e11fa1d68616f86450593f12f6aeeb1adc14
            @Override
            public void onClick(View view) {
                try{
                    File file = new File(pathactual);
                    if(file.exists()){
                        file.delete();
                    }
                }catch (Exception e){
                   e.printStackTrace();
                }

            }
        });
<<<<<<< HEAD
        btnImprimir.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                    String[] aux = pathtemp.split("/");
                    nombre = aux[aux.length];
                StringRequest str = new StringRequest(Request.Method.POST, url,
                        new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {

                            }
                        }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(getApplicationContext(),"Error:"+error.toString(),Toast.LENGTH_LONG).show();
                    }
                }) {
                    @Override
                    protected Map<String,String> getParams(){
                        Map<String,String> params = new HashMap<>();
                        params.put("nombre",nombre);
                        return params;
                    }
                };
                rq.add(str);
                Toast.makeText(getApplicationContext(),"Imagen Imprimiendose...",Toast.LENGTH_SHORT).show();
            }
        });
=======

        solicitarPermisos();
>>>>>>> 88c9e11fa1d68616f86450593f12f6aeeb1adc14
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

    @Override
    public void onClick(View view) {
        Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
        intent.setType("image/");
        startActivityForResult(intent,0);

    }
    @Override
    protected  void onActivityResult(int requestCode,int resultCode,Intent data){
        super.onActivityResult(requestCode,resultCode,data);
        if(resultCode==RESULT_OK){
            Uri path =data.getData();
            Log.e(pathactual,"Odiomibida");
            imagen.setImageURI(path);
        }
    }
}
