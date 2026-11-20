// XnaReflectiveReader.cs -- a type with no reader of its own read the way XNA 4.0 wrote it.
//
// XNA's content pipeline writes such a type member by member: its base type's members first, then its own properties,
// then its own fields, each in declaration order. A property is written when it has a public getter and a public
// setter, a field when it is public and not readonly -- or either when it carries [ContentSerializer]; never one with
// [ContentSerializerIgnore]. MonoGame's ReflectiveReader also reads a property whose setter is not public
// (`public bool Initialised { get; private set; }`), so from there on every member gets the next one's bytes. The
// port step renames XNA's ReflectiveReader in the game's .xnb files to this one.
using System;
using System.Collections.Generic;
using System.Reflection;
using Microsoft.Xna.Framework.Content;

namespace XnaPort
{
    public class XnaReflectiveReader<T> : ContentTypeReader
    {
        const BindingFlags Declared = BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.DeclaredOnly;

        ContentTypeReader baseReader;
        readonly List<Action<ContentReader, object>> members = new List<Action<ContentReader, object>>();

        public XnaReflectiveReader() : base(typeof(T)) { }

        public override bool CanDeserializeIntoExistingObject => TargetType.IsClass;

        protected override void Initialize(ContentTypeReaderManager manager)
        {
            Type b = TargetType.BaseType;
            if (b != null && b != typeof(object) && b != typeof(ValueType))
                baseReader = manager.GetTypeReader(b);
            foreach (PropertyInfo p in TargetType.GetProperties(Declared))
                Add(manager, p, p.PropertyType);
            foreach (FieldInfo f in TargetType.GetFields(Declared))
                Add(manager, f, f.FieldType);
        }

        void Add(ContentTypeReaderManager manager, MemberInfo member, Type type)
        {
            if (member.GetCustomAttribute<ContentSerializerIgnoreAttribute>() != null)
                return;
            var attribute = member.GetCustomAttribute<ContentSerializerAttribute>();
            Func<object, object> get;
            Action<object, object> set;
            if (member is PropertyInfo p)
            {
                if (p.GetIndexParameters().Length > 0 || p.GetMethod == null)
                    return;
                if (attribute == null && (!p.GetMethod.IsPublic || p.SetMethod == null || !p.SetMethod.IsPublic))
                    return;
                get = p.GetValue;
                set = p.SetMethod == null ? null : p.SetValue;
            }
            else
            {
                var f = (FieldInfo)member;
                if (attribute == null && (!f.IsPublic || f.IsInitOnly))
                    return;
                if (f.GetCustomAttribute<System.Runtime.CompilerServices.CompilerGeneratedAttribute>() != null)
                    return;                                         // a property's backing field: the property is the member
                get = f.GetValue;
                set = f.SetValue;
            }
            if (attribute != null && attribute.SharedResource)
            {
                members.Add((input, o) => input.ReadSharedResource<object>(v => set?.Invoke(o, v)));
                return;
            }
            ContentTypeReader reader = manager.GetTypeReader(type);
            if (set == null)                                        // a [ContentSerializer] collection read into the one it has
                members.Add((input, o) => input.ReadObject(reader, get(o)));
            else
                members.Add((input, o) => set(o, input.ReadObject(reader, reader.CanDeserializeIntoExistingObject ? get(o) : null)));
        }

        protected override object Read(ContentReader input, object existing)
        {
            object o = existing ?? Activator.CreateInstance(TargetType, true);   // a struct is filled boxed
            if (baseReader != null)
                input.ReadRawObject(baseReader, o);
            foreach (var m in members)
                m(input, o);
            return o;
        }
    }
}
