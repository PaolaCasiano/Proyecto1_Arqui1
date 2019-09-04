package com.example.arqui1_p1;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    Toolbar toolbar;
    Button btn_draw;
    Button btn_galery;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        btn_draw = findViewById(R.id.btn_draw);
        btn_galery = findViewById(R.id.btn_galery);

    }

    public void dibujoNuevo(View view){
        Intent i = new Intent(this,Main2Activity.class);
        startActivity(i);
    }
    public void galeria(View view){
        Toast.makeText(view.getContext(),"Abriendo galería",Toast.LENGTH_SHORT);
    }
}
