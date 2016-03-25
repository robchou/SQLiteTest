package com.androidhive.sqlitetest;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();
    Context context = MainActivity.this;
    TextView tvMsg;
    DataBaseHelper dataBaseHelper;
    private static final String DATABASE_NAME = "duomai.db";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        dataBaseHelper = new DataBaseHelper();

        tvMsg = (TextView)findViewById(R.id.tv_msg);
        tvMsg.setText(dataBaseHelper.stringFromJNI());

        File dbFile = new File(context.getFilesDir(), DATABASE_NAME);
        dataBaseHelper.writeStringToFile(dbFile.getAbsolutePath(), "Hello, Duomai");
        Log.e(TAG, dbFile.getAbsolutePath());
    }
}
