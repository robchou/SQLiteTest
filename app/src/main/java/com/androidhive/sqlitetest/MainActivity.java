package com.androidhive.sqlitetest;

import android.content.Context;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();
    Context context = MainActivity.this;
    TextView tvMsg;
    NativeTest nativeTest;
    private static final String DATABASE_NAME = "duomai.db";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        nativeTest = new NativeTest();

        tvMsg = (TextView)findViewById(R.id.tv_msg);
        tvMsg.setText(nativeTest.stringFromJNI());

        File dbFile = new File(context.getFilesDir(), DATABASE_NAME);
        nativeTest.writeStringToFile(dbFile.getAbsolutePath(), "Hello, Duomai");
        Log.e(TAG, dbFile.getAbsolutePath());

        writeStringToFile("hello", "Hello World!");
        getTempFile(context, "hello");
    }

    //Write string to private file
    public void writeStringToFile(String file, String content) {
        FileOutputStream outputStream;

        try {
            outputStream = openFileOutput(file, Context.MODE_PRIVATE);
            outputStream.write(content.getBytes());
            outputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    //Generate tmp file
    public File getTempFile(Context context, String url) {
        File file = null;

        try {
            String fileName = Uri.parse(url).getLastPathSegment();
            file = File.createTempFile(fileName, null, context.getCacheDir());
        } catch (IOException e) {
            // Error while creating file
            e.printStackTrace();
        }

        return file;
    }
}
