using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour {
	public float speed = 6;
	public float rotationSpeed = 60;
	private CharacterController controller;
	public static int num = 0;


	// Use this for initialization
	void Start () {
		controller = GetComponent<CharacterController> ();
		Vector3 position = new Vector3(Random.Range(-100.0F, 100.0F), 0, Random.Range(-100.0F, 100.0F));
		controller.Move (position);
		if (Player.num < 5) {
			GameObject cubez = (GameObject)Instantiate (Resources.Load ("Cubez", typeof(GameObject)));
			cubez.name = "Cub_" + num;
			//cubez = "Cub_" + num;
		}
		num ++;
	}
	
	// Update is called once per frame
	void Update () {
		float vertical = 0;
		float horizontal = 0;
		vertical = Input.GetAxis ("Vertical");
		horizontal = Input.GetAxis ("Horizontal");
		if (vertical == 0 && horizontal == 0) return;
		transform.Rotate (0, rotationSpeed * Time.deltaTime * horizontal, 0);
		controller.Move (transform.forward * speed * Time.deltaTime * vertical);
	}
}
