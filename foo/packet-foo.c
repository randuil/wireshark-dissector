 #include "config.h"
#include <epan/packet.h>

//Protocol port
#define FOO_PORT 1234

//Flags
#define FOO_START_FLAG      0x01
#define FOO_END_FLAG        0x02
#define FOO_PRIORITY_FLAG   0x04
#define FOO_INFO_FLAG   	0x08


//Protocol handelrs, will be set when the dissector is registered within the main program
static int proto_foo = -1;
static int hf_foo_pdu_type = -1;
static int hf_foo_hdr_version = -1;

//Flags handlers
static int hf_foo_flags = -1;
static int hf_foo_startflag = -1;
static int hf_foo_endflag = -1;
static int hf_foo_infoflag = -1;
static int hf_foo_priorityflag = -1;

//Boolean handler
static int hf_foo_bool = -1;

//Payload length handler
static int hf_foo_pl_len = -1;

//Payload handler
static int hf_foo_payload = -1;

//End of protocol subtree handler
static gint ett_foo = -1;

//packet types
static const value_string packettypenames[] = {
    { 1, "Initialise" },
    { 2, "Terminate" },
    { 3, "Data" },
    { 0, NULL }
};

static int
dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
{
	gint offset = 0;
	/* Get packet type */
	guint8 packet_type = tvb_get_guint8(tvb, 1);
	
	/*Set protocol column to FOO*/
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "FOO");
    
    /* Clear the info column */
    col_clear(pinfo->cinfo,COL_INFO);
    
    /* Add packet type to info column */
    col_add_fstr(pinfo->cinfo, COL_INFO, "Type %s",val_to_str(packet_type, packettypenames, "Unknown (0x%02x)"));
    
    /* Add "FOO" subtree*/
	proto_item *ti = proto_tree_add_item(tree, proto_foo, tvb, 0, -1, ENC_NA);
	/* Add packet type to "FOO" subtree */
	proto_item_append_text(ti, ", Type %s",
        val_to_str(packet_type, packettypenames, "Unknown (0x%02x)"));
    
    /* Add "FOO" subtree to "main" tree */
    proto_tree *foo_tree = proto_item_add_subtree(ti, ett_foo);
    
    /* Add "version" field to "main" tree */
    proto_tree_add_item(foo_tree, hf_foo_hdr_version, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset += 1;
    
    /* Add packet "type" field to "main" tree */
    proto_tree_add_item(foo_tree, hf_foo_pdu_type, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset += 1;
    
    /* Add packet "flags" field to "main" tree*/
    static int* const bits[] = {
            &hf_foo_startflag,
            &hf_foo_endflag,
            &hf_foo_priorityflag,
            &hf_foo_infoflag,
            NULL
        };
    proto_tree_add_bitmask(foo_tree, tvb, offset, hf_foo_flags, ett_foo, bits, ENC_BIG_ENDIAN);
	offset += 1;
	
	/* Add packet "boolean" field to "main" tree*/
	proto_tree_add_item(foo_tree, hf_foo_bool, tvb, offset, 1, ENC_BIG_ENDIAN);
	offset += 1;
    
    /* Add packet "payload length" field to "main" tree*/
    proto_tree_add_item(foo_tree, hf_foo_pl_len, tvb, offset, 4, ENC_BIG_ENDIAN);
    offset += 4;
    
    /* Add packet "payload" field to "main" tree*/
    proto_tree_add_item(foo_tree, hf_foo_payload, tvb, offset, -1, ENC_BIG_ENDIAN);
    
    return tvb_captured_length(tvb);
}

//Used to register protocol in wireshark
void
proto_register_foo(void)
{
    static hf_register_info hf[] = {
        { &hf_foo_pdu_type,						//The node’s index,
            { "FOO Packet Type", "foo.type",    //The item’s label, The item’s abbreviated name
            FT_UINT8, BASE_DEC,					//The iems's type, print as decimal number
            NULL, 0x0,
            NULL, HFILL }
        },
        { &hf_foo_hdr_version, 
			{ "FOO Version", "foo.version",
			FT_UINT8, BASE_DEC,
			NULL, 0x0,
			NULL, HFILL}
		},
		{ &hf_foo_flags,
			{"FOO FLAGS", "foo.flags",
			FT_UINT8, BASE_HEX,
			NULL, 0x0,
			NULL, HFILL}
		},
		{ &hf_foo_startflag,
            { "FOO Start Flags", "foo.flags.start",
            FT_BOOLEAN, 8,
            NULL, FOO_START_FLAG,
            NULL, HFILL }
        },
        { &hf_foo_endflag,
            { "FOO End Flags", "foo.flags.end",
            FT_BOOLEAN, 8,
            NULL, FOO_END_FLAG,
            NULL, HFILL }
        },
        { &hf_foo_priorityflag,
            { "FOO Priority Flags", "foo.flags.priority",
            FT_BOOLEAN, 8,
            NULL, FOO_PRIORITY_FLAG,
            NULL, HFILL }
        },
        { &hf_foo_infoflag,
            { "FOO Info Flags", "foo.flags.info",
            FT_BOOLEAN, 8,
            NULL, FOO_INFO_FLAG,
            NULL, HFILL }
        },
         { &hf_foo_bool,
            { "FOO Header Boolean", "foo.hdr.bool",
            FT_BOOLEAN, BASE_NONE,
            NULL, 0x0,
            NULL, HFILL }
        },
        { &hf_foo_pl_len,
            { "FOO Payload Length", "foo.pl_len",
            FT_UINT8, BASE_DEC,
            NULL, 0x0,
            NULL, HFILL }
        },
		{ &hf_foo_payload,
            { "FOO Payload", "foo.payload",
            FT_STRING, BASE_NONE,
            NULL, 0x0,
            NULL, HFILL }
        }
       
    };

    /* Setup protocol subtree array */
    static gint *ett[] = {
        &ett_foo
    };

    proto_foo = proto_register_protocol (
        "FOO Protocol", /* name       */
        "FOO",          /* short_name */
        "foo"           /* filter_name*/
        );

    proto_register_field_array(proto_foo, hf, array_length(hf));
    proto_register_subtree_array(ett, array_length(ett));
}


//handoff routine associates a protocol handler with the protocol’s traffic
void
proto_reg_handoff_foo(void)
{
    static dissector_handle_t foo_handle;

    foo_handle = create_dissector_handle(dissect_foo, proto_foo);
    dissector_add_uint("udp.port", FOO_PORT, foo_handle);
}
