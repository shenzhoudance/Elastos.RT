package com.elastos.rpcserver;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private static final int MSG_CONNECT_CHANGED = 2003;
    private static final int MSG_FRIEND_REQUEST = 2004;
    private static final int MSG_FRIEND_CHANGED = 2005;
    private static final int MSG_CHANGE_SWITCH1 = 2006;
    private static final int MSG_CHANGE_SWITCH2 = 2007;

    private static boolean sServiceAdded = false;

    private LinearLayout mConnecting;
    private LinearLayout mConnected;

    private TextView mSwitch1;
    private TextView mSwitch2;

    private RecyclerView mFriendList;

    private FriendAdapter mAdapter;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                customScan();
            }
        });


        mConnecting = findViewById(R.id.layout_connecting);
        mConnecting.setVisibility(View.VISIBLE);

        mConnected = findViewById(R.id.layout_connected);
        mConnected.setVisibility(View.GONE);

        mSwitch1 = findViewById(R.id.switch_1);
        mSwitch2 = findViewById(R.id.switch_2);

        initList();

        startCarrier(getFilesDir().getAbsolutePath());
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        stopServiceManager();
        stopCarrier();
    }

    private void initList() {
        mFriendList = findViewById(R.id.list_friends);
        mFriendList.setLayoutManager(new LinearLayoutManager(
                this, LinearLayoutManager.VERTICAL, false));

        ArrayList<Friend> list = new ArrayList<>();
        mAdapter = new FriendAdapter(list);
        mAdapter.setOnClickListener(new FriendAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(View view, final Friend friend) {
                Log.d("Friend", "click friend:" + friend.mUid);
            }
            
            @Override
            public void onItemDeleteMenuClick(View view, final Friend friend) {
                AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this)
                        .setTitle("Delete Friend")
                        .setMessage("delete " + friend.mUid)
                        .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                dialogInterface.dismiss();
                                int ret = deleteFriend(friend.mUid);
                                if (ret < 0) return;

                                mAdapter.deleteFriend(friend);
                            }
                        })
                        .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialogInterface, int i) {
                                dialogInterface.dismiss();
                            }
                        });
                builder.create().show();
            }
        });
        mFriendList.setAdapter(mAdapter);
        mFriendList.addItemDecoration(new ItemDecoration(this, LinearLayoutManager.VERTICAL));

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            String address = getAddress();
            if (address == null || address.isEmpty()) {
                Log.d("MainActivity", "carrier is not ready");
                return true;
            }

            Intent intent = new Intent(MainActivity.this, QRCodeActivity.class);
            intent.putExtra("myAddress", address);
            startActivity(intent);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        IntentResult intentResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
        if(intentResult != null) {
            if(intentResult.getContents() == null) {
                Toast.makeText(this,"内容为空",Toast.LENGTH_LONG).show();
            } else {
                Toast.makeText(this,"扫描成功",Toast.LENGTH_LONG).show();
                String address = intentResult.getContents();
                Log.d("MainActivity", "scan address: " + address);
                alertAddFriend(address);
            }
        } else {
            super.onActivityResult(requestCode,resultCode,data);
        }
    }


    private void alertAddFriend(final String address) {
        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this)
                .setTitle("Add Friend")
                .setMessage(address)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        dialogInterface.dismiss();
                        addFriend(address);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        dialogInterface.dismiss();
                    }
                });
        builder.create().show();
    }


    public void customScan() {
        new IntentIntegrator(this)
                .setOrientationLocked(false)
                .setCaptureActivity(SCanActivity.class)
                .initiateScan();
    }


    public void OnConnectionChanged(boolean connected)
    {
        Message msg = new Message();
        msg.what = MSG_CONNECT_CHANGED;
        Bundle bundle = new Bundle();
        bundle.putBoolean("connection", connected);
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    public void OnFriendRequest(String uid, String hello)
    {
        Message msg = new Message();
        msg.what = MSG_FRIEND_REQUEST;
        Bundle bundle = new Bundle();
        bundle.putString("friendUid", uid);
        bundle.putString("friendHello", hello);
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    public void OnFriendConnectionChanged(String uid, boolean online)
    {
        Log.d("MainActivity", "OnFriendConnectionChanged: " + uid + " online: " + online);
        Message msg = new Message();
        msg.what = MSG_FRIEND_CHANGED;
        Bundle bundle = new Bundle();
        bundle.putString("friendUid", uid);
        bundle.putBoolean("friendOnline", online);
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    Handler mHandler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message message) {
            switch (message.what) {
                case MSG_CONNECT_CHANGED:
                    handleConnectionChanged(message);
                    break;
                case MSG_FRIEND_REQUEST:
                    handleFriendRequest(message);
                    break;
                case MSG_FRIEND_CHANGED:
                    handleFriendConnectionChanged(message);
                    break;
                case MSG_CHANGE_SWITCH1:
                    handleSwitch1(message);
                    break;
                case MSG_CHANGE_SWITCH2:
                    handleSwitch2(message);
                    break;
                default:
                    break;
            }
            return false;
        }
    });

    private void handleConnectionChanged(Message msg) {
        Bundle bundle = msg.getData();
        Boolean connected = bundle.getBoolean("connection");
        if (connected) {
            mConnecting.setVisibility(View.GONE);
            mConnected.setVisibility(View.VISIBLE);
            ArrayList<Friend> list = getFriendList();
            mAdapter.clearFriends();
            mAdapter.addFriendList(list);

            startServiceManager();
            if (!sServiceAdded) {
                int ret = addService("s");
                Log.d(TAG, " === add Service ret:" + ret);
                sServiceAdded = true;
            }
        } else{
            mConnecting.setVisibility(View.VISIBLE);
            mConnected.setVisibility(View.GONE);

            mAdapter.clearFriends();
        }
    }


    private void handleFriendRequest(Message msg) {
        Bundle bundle = msg.getData();
        final String uid = bundle.getString("friendUid");
        String hello = bundle.getString("friendHello");

        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this)
                .setTitle("Friend Request")
                .setMessage(uid + " say: " + hello)
                .setPositiveButton("accept", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        if (acceptFriend(uid) == 0) {
                            mAdapter.addFriend(new Friend(uid, false));
                        }
                    }
                })
                .setNegativeButton("ignore", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
        builder.create().show();
    }

    private void handleFriendConnectionChanged(Message msg) {
        Bundle bundle = msg.getData();
        String uid = bundle.getString("friendUid");
        Boolean online = bundle.getBoolean("friendOnline");
        mAdapter.friendStatusChanged(uid, online, false);
    }

    private void handleSwitch1(Message msg) {
        Bundle bundle = msg.getData();
        boolean on = bundle.getBoolean("switch");
        if (on) {
            mSwitch1.setText("ON");
            mSwitch1.setTextColor(getResources().getColor(R.color.colorSwitchOn));
        } else {
            mSwitch1.setText("OFF");
            mSwitch1.setTextColor(getResources().getColor(R.color.colorSwitchOff));
        }
    }

    private void handleSwitch2(Message msg) {
        Bundle bundle = msg.getData();
        boolean on = bundle.getBoolean("switch");
        if (on) {
            mSwitch2.setText("ON");
            mSwitch2.setTextColor(getResources().getColor(R.color.colorSwitchOn));
        } else {
            mSwitch2.setText("OFF");
            mSwitch2.setTextColor(getResources().getColor(R.color.colorSwitchOff));
        }
    }

    public void switch1(boolean on) {
        Message msg = new Message();
        msg.what = MSG_CHANGE_SWITCH1;
        Bundle bundle = new Bundle();
        bundle.putBoolean("switch", on);
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    public void switch2(boolean on) {
        Message msg = new Message();
        msg.what = MSG_CHANGE_SWITCH2;
        Bundle bundle = new Bundle();
        bundle.putBoolean("switch", on);
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void startCarrier(String path);
    public native void stopCarrier();
    public native int addFriend(String uid);
    public native int deleteFriend(String uid);
    public native ArrayList<Friend> getFriendList();
    public native int acceptFriend(String uid);
    public native String getAddress();

    public native void startServiceManager();
    public native void stopServiceManager();
    public native int addService(String name);
}
