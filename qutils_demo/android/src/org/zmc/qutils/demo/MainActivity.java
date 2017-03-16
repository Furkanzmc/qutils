package org.zmc.qutils.demo;

import android.util.Log;
import android.os.Bundle;
import android.content.Intent;

import org.zmc.qutils.QutilsActivity;

public class MainActivity extends QutilsActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        m_NotificationClient.setSmallIcon(R.drawable.icon);
    }
}
