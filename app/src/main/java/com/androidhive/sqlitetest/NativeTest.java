package com.androidhive.sqlitetest;

/**
 * Created by robin on 3/25/16.
 */
public class NativeTest {
    static {
        System.loadLibrary("im");
    }

    public native void NativeTest();
}
