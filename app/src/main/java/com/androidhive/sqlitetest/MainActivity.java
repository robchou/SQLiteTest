package com.androidhive.sqlitetest;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();
    Context context = MainActivity.this;
    TextView tvMsg;
    DataBaseHelper dataBaseHelper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        dataBaseHelper = new DataBaseHelper();

        tvMsg = (TextView)findViewById(R.id.tv_msg);
        tvMsg.setText(dataBaseHelper.stringFromJNI());
    }
}
