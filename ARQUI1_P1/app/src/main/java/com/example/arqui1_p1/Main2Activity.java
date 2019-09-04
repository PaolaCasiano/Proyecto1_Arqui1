package com.example.arqui1_p1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class Main2Activity extends AppCompatActivity {

    TextView txt_nombre;
    Button btn_continuar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        txt_nombre = findViewById(R.id.txt_nombre);
        btn_continuar = findViewById(R.id.btn_continuar);
    }
    public void dibujar(View view){
        if(txt_nombre.getText().toString().length()>0){
            Intent i = new Intent(this, Main3Activity.class);
            i.putExtra("dato",txt_nombre.getText().toString());
            startActivity(i);
        }
        else{
            Log.println(Log.ERROR,"Error","hubo un error");
            Toast.makeText(this,"Debe ingresar un nombre para el dibujo",Toast.LENGTH_SHORT);
        }

    }
}
