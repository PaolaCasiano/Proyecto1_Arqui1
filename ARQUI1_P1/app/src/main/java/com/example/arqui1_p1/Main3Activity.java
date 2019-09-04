package com.example.arqui1_p1;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;

import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;
import androidx.annotation.NonNull;

import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class Main3Activity extends AppCompatActivity {
    String dato;
    TextView txt_nombre;
    Lienzo lienzo;
    FloatingActionButton fbNegro;
    FloatingActionButton fbAzul;
    FloatingActionButton fbRojo;

    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            switch (item.getItemId()) {
                case R.id.navigation_lapiz:
                    return true;
                case R.id.navigation_borrador:
                    lienzo.setBorrado();
                    return true;
                case R.id.navigation_imprimir:
                    //Metodo para guardar la imagen y mandar a imprimir
                    return true;
                case R.id.navigation_nuevo:
                    lienzo.nuevoLienzo();
                    return true;
            }
            return false;
        }
    };

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

        fbNegro= findViewById(R.id.floatingActionButtonNegro);
        fbAzul= findViewById(R.id.floatingActionButtonAzul);
        fbRojo= findViewById(R.id.floatingActionButtonRojo);

        fbNegro.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                lienzo.setColor(Color.BLACK);
            }
        });
        fbNegro.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                lienzo.setColor(Color.BLUE);
            }
        });
        fbAzul.setOnClickListener(new View.OnClickListener() {
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
}
