//
//  heheheApp.swift
//  hehehe
//
//  Created by 吳驊祐 on 2021/3/15.
//

import SwiftUI

@main
struct heheheApp: App {
    var body: some Scene {
        DocumentGroup(newDocument: heheheDocument()) { file in
            ContentView(document: file.$document)
        }
    }
}
