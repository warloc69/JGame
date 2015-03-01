using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour 
{
	public static bool isActive;
    public static bool isMove =false;
    public float speed = 10.0F;
    public float rotationSpeed = 10.0F;
    float x1;
    float y1;
	void Start () 
	{
        
	}
  
	
	// Update is called once per frame
	void FixedUpdate () 
	{
		//if(!isActive)
		//	return;
        float z = Input.GetAxis("Vertical") * speed;
        float x = Input.GetAxis("Horizontal") * rotationSpeed;
        x *= Time.deltaTime;
        z *= Time.deltaTime;
        transform.Translate(x, 0, z);
        x1 +=  Input.GetAxis("Mouse X") * speed * 0.5f;
        y1 -= Input.GetAxis("Mouse Y") * speed * 0.5f;
        
       
        //Rotate the camera to those angles 
        var rotation = Quaternion.Euler(y1,x1,0);
        transform.rotation = rotation;
		// fire
	//	if(Input.GetMouseButtonDown(0))
	//	{
	//		JavaClient.sendPacket(PacketBuilder.gameObjectFire((short) 2, this.transform.forward));
	//	}

		
	//	JavaClient.sendPacket(PacketBuilder.gameObjectMove(this.transform.position, this.transform.rotation));
	}
}
