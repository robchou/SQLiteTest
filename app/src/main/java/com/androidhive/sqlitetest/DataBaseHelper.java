package com.androidhive.sqlitetest;

/**
 * Created by robin on 3/25/16.
 */
public class DataBaseHelper {
    static {
        System.loadLibrary("im");
    }

    public native String stringFromJNI();
}
